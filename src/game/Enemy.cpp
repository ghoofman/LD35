#include "Enemy.h"
#include "Global.h"
#include "cards\wolf.h"
#include "cards\energy.h"
#include "cards\skeleton.h"
#include "cards\devilshowl.h"
#include "cards/CardType.h"

void EnemyInit(Enemy* enemy, OPphysXControllerManager* manager, const OPchar* model, const OPchar* texture) {

	enemy->deck.Init(20);
	//deck.AddCard("Energy.png", EnergyCard, AlwaysAvailable);
	//deck.AddCard("Energy.png", EnergyCard, AlwaysAvailable);
	//deck.AddCard("Skel.png", SkelCard, AlwaysAvailable);
	//deck.AddCard("Energy.png", EnergyCard, AlwaysAvailable);
	//deck.AddCard("Energy.png", EnergyCard, AlwaysAvailable);
	//deck.AddCard("Energy.png", EnergyCard, AlwaysAvailable);
	//deck.AddCard("WolfCard.png", WolfCard, AlwaysAvailable);
	//deck.AddCard("Energy.png", EnergyCard, AlwaysAvailable);
	//deck.AddCard("Energy.png", EnergyCard, AlwaysAvailable);
	//deck.AddCard("Energy.png", EnergyCard, AlwaysAvailable);
	//deck.AddCard("Energy.png", EnergyCard, AlwaysAvailable);
	//deck.AddCard("DevilsHowlCard.png", DevilsHowl, DevilsHowlCardAvailable);
	for (ui32 i = 0; i < 10; i++) {
		enemy->deck.AddCard(CardEnergy());
	}
	for (ui32 i = 0; i < 2; i++) {
		enemy->deck.AddCard(CardWolf());
	}
	for (ui32 i = 0; i < 1; i++) {
		enemy->deck.AddCard(CardDevilsHowl());
	}
	for (ui32 i = 0; i < 2; i++) {
		enemy->deck.AddCard(CardSkeleton());
	}
	enemy->deck.Shuffle();


	enemy->character.model.Init(model, texture);
	enemy->character.skeleton = *OPskeletonCopy((OPskeleton*)OPcmanLoadGet("ld35person.opm.skel"));
	enemy->character.idle = *(OPskeletonAnimation*)OPcmanLoadGet("ld35person.opm.Take 001.anim");
	enemy->character.walk = *(OPskeletonAnimation*)OPcmanLoadGet("ld35person.opm.Walk.anim");
	enemy->character.attack = *(OPskeletonAnimation*)OPcmanLoadGet("ld35person.opm.Attack.anim");
	enemy->character.activeAnimation = &enemy->character.idle;

	enemy->character.position = OPvec3Create(100, 0, 0);
	enemy->character.rotation = OPVEC3_ZERO;
	enemy->character.scale = OPVEC3_ONE;
	enemy->character.radius = 5.0f;
	enemy->character.attackRange = 10.0f;
	enemy->character.health = 100;
	enemy->character.energy = 0;
	enemy->character.damage = 1;
	enemy->character.knockback = 1.0;
	enemy->character.speed = 1.0;

	enemy->character.physics.maxSpeed = 2.0;
	enemy->character.physics.maxThrowbackSpeed = 10.0;
	enemy->character.physics.acceleration = 10.0;
	enemy->character.physics.drag = 8.0;
	enemy->character.physics.throwbackDrag = 12.0;
	enemy->character.physics.gravity = -9.8;
	enemy->character.physics.jumpSpeed = 1.22 * 4;

	enemy->character.isAttacking = 0;
	enemy->character.attackTime = 0;
	enemy->character.maxAtackTime = 500;

	enemy->character.velocity = OPVEC3_ZERO;
	enemy->character.throwbackVelocity = OPVEC3_ZERO;
	enemy->character.target = OPVEC3_ZERO;

	enemy->lastCardDrawn = 3000;
	enemy->actionTime = 1500;
	enemy->lastMove = enemy->actionTime;


	OPphysXMaterial* material = OPphysXCreateMaterial(0.8, 0.8, 0.6);
	enemy->physXController = OPphysXControllerCreate(manager, material, 9, enemy->character.radius, NULL, NULL, NULL);
	OPphysXControllerSetPos(enemy->physXController, enemy->character.position);
}

void EnemyAI(Enemy* enemy, Scene* scene) {

	i32 cardInd = -1;
	i8 activate = 1;

	if (cardInd == -1) {
		if (enemy->character.energy > 0) {
			// Look for a card to play
			for (ui32 i = 0; i < enemy->deck.handCount; i++) {

				// If it's a shapeshift card, and we're already that form, ignore it
				if (enemy->deck.hand[i].Desc.Shapeshift && enemy->character.form == enemy->deck.hand[i].Desc.Form) continue;

				// Look for a non energy card, that we have enough energy to play
				if (enemy->deck.hand[i].Desc.Type != CardType::Energy
					&& enemy->deck.hand[i].Desc.Cost <= enemy->character.energy
					&& enemy->deck.hand[i].Desc.Available(&enemy->character, scene)) {
					cardInd = i;
					break;
				}
			}
		}
	}

	if(cardInd == -1) {
		// look for energy card
		for (ui32 i = 0; i < enemy->deck.handCount; i++) {
			if (enemy->deck.hand[i].Desc.Type == CardType::Energy) {
				cardInd = i;
				break;
			}
		}
	}

	// At this point we've hit a no energy block, discard a card
	if (cardInd == -1 && enemy->deck.handCount == enemy->deck.handMax) {
		// find the highest costing card
		ui8 cost = 0;
		for (ui32 i = 0; i < enemy->deck.handCount; i++) {
			if (enemy->deck.hand[i].Desc.Cost > cost) {
				cardInd = i;
				cost = enemy->deck.hand[i].Desc.Cost;
			}
		}
		activate = 0;
	}

	// There's an energy card in our hand
	if (cardInd > -1) {
		if (cardInd == enemy->deck.activeCard) {
			if (activate) {
				enemy->deck.ActivateCard(&enemy->character, scene);
			}
			else {
				enemy->deck.KillCard();
			}
			enemy->lastMove = enemy->actionTime;
		}
		else if (cardInd < enemy->deck.activeCard) {
			enemy->deck.activeCard--;
			enemy->lastMove = enemy->actionTime;
		}
		else if (cardInd > enemy->deck.activeCard) {
			enemy->deck.activeCard++;
			enemy->lastMove = enemy->actionTime;
		}
	}

}

void EnemyUpdate(Enemy* enemy, Scene* scene, OPtimer* timer) {
	OPfloat dt = timer->Elapsed / 1000.0f;

	enemy->lastCardDrawn -= timer->Elapsed;
	if (enemy->lastCardDrawn <= 0 && enemy->deck.handCount < enemy->deck.handMax) {
		Card* card = enemy->deck.DrawCard();
		enemy->lastCardDrawn = 3000;
	}

	if (enemy->deck.handCount > 0) {
		enemy->lastMove -= timer->Elapsed;
		if (enemy->lastMove < 0) {
			EnemyAI(enemy, scene);
		}
	}


	enemy->character.attackTime += timer->Elapsed;

	if (enemy->character.attackTime > enemy->character.maxAtackTime) {
		enemy->character.activeAnimation = &enemy->character.walk;
	}


	if (enemy->character.attackTime > enemy->character.maxAtackTime ) {

		// We can attack, see if there's a player within range

		for (ui32 j = 0; j < scene->playerCount; j++) {
			OPvec3 relPos = scene->players[j].character.position - enemy->character.position;
			OPfloat radSumSqr = enemy->character.radius + scene->players[j].character.radius;
			OPfloat distSqr = OPvec3Dot(relPos, relPos);
			f32 range = radSumSqr * radSumSqr;
			f32 attackRange = radSumSqr * enemy->character.attackRange * 4;
			if (distSqr <= attackRange) {
				enemy->character.isAttacking = 1;
				enemy->character.attackTime = 0;
				enemy->character.canAttack = 0;
				enemy->character.activeAnimation = &enemy->character.attack;
				OPfmodPlay(OPfmodLoad("Audio/woosh.wav"));
			}
		}
	}

	if (enemy->character.isAttacking && enemy->character.attackTime > enemy->character.maxAtackTime) {
		enemy->character.isAttacking = 0;
	}



	OPvec3 dir = OPvec3Norm(enemy->character.target - enemy->character.position);



	OPvec2 planerVelocity = {
		enemy->character.velocity.x,
		enemy->character.velocity.z
	};
	OPvec2 velDragDir = OPvec2Norm(planerVelocity) * -enemy->character.physics.drag;

	if (enemy->character.velocity.x > 0) {
		enemy->character.velocity.x += velDragDir.x * dt;
		if (enemy->character.velocity.x < 0) enemy->character.velocity.x = 0;
	}
	else if (enemy->character.velocity.x < 0) {
		enemy->character.velocity.x += velDragDir.x * dt;
		if (enemy->character.velocity.x > 0) enemy->character.velocity.x = 0;
	}

	if (enemy->character.velocity.z > 0) {
		enemy->character.velocity.z += velDragDir.y * dt;
		if (enemy->character.velocity.z < 0) enemy->character.velocity.z = 0;
	}
	else if (enemy->character.velocity.z < 0) {
		enemy->character.velocity.z += velDragDir.y * dt;
		if (enemy->character.velocity.z > 0) enemy->character.velocity.z = 0;
	}

	OPfloat speed = 1.0 + enemy->character.speed / 6.0;
	OPfloat len = OPvec3Len(dir);

	OPvec3 forward = enemy->character.target - enemy->character.position;
	forward.y = 0;
	forward = OPvec3Norm(forward);

	OPvec3 left = OPvec3Cross(forward, OPVEC3_UP);

	if (len > 0) {

		//OPvec3 acceleration = forward * -dir.z * enemy->physics.acceleration * speed;
		//acceleration += left * -dir.x * enemy->physics.acceleration * speed;

		OPvec3 acceleration = dir *  enemy->character.physics.acceleration * speed;

		enemy->character.velocity += acceleration * dt;

		enemy->character.rotation = OPvec3Create(0, OPpi_2 + OPatan2(dir.x, -dir.z), 0);
	}

	OPvec2 throwbackPlanerVelocity = {
		enemy->character.throwbackVelocity.x,
		enemy->character.throwbackVelocity.z
	};

	if (OPvec2Len(throwbackPlanerVelocity) > enemy->character.physics.maxThrowbackSpeed) {
		throwbackPlanerVelocity = OPvec2Norm(throwbackPlanerVelocity) * enemy->character.physics.maxThrowbackSpeed;
		enemy->character.throwbackVelocity.x = throwbackPlanerVelocity.x;
		enemy->character.throwbackVelocity.z = throwbackPlanerVelocity.y;
	}
	OPvec2 throwbackVelDragDir = OPvec2Norm(throwbackPlanerVelocity) * -enemy->character.physics.throwbackDrag;

	if (enemy->character.throwbackVelocity.x > 0) {
		enemy->character.throwbackVelocity.x += throwbackVelDragDir.x * dt;
		if (enemy->character.throwbackVelocity.x < 0) enemy->character.throwbackVelocity.x = 0;
	}
	else if (enemy->character.throwbackVelocity.x < 0) {
		enemy->character.throwbackVelocity.x += throwbackVelDragDir.x * dt;
		if (enemy->character.throwbackVelocity.x > 0) enemy->character.throwbackVelocity.x = 0;
	}

	if (enemy->character.throwbackVelocity.z > 0) {
		enemy->character.throwbackVelocity.z += throwbackVelDragDir.y * dt;
		if (enemy->character.throwbackVelocity.z < 0) enemy->character.throwbackVelocity.z = 0;
	}
	else if (enemy->character.throwbackVelocity.z < 0) {
		enemy->character.throwbackVelocity.z += throwbackVelDragDir.y * dt;
		if (enemy->character.throwbackVelocity.z > 0) enemy->character.throwbackVelocity.z = 0;
	}

	enemy->character.velocity.y = enemy->character.physics.gravity * dt;
	planerVelocity.x = enemy->character.velocity.x;
	planerVelocity.y = enemy->character.velocity.z;
	if (OPvec2Len(planerVelocity) > enemy->character.physics.maxSpeed * speed) {
		planerVelocity = OPvec2Norm(planerVelocity) * enemy->character.physics.maxSpeed * speed;
		enemy->character.velocity.x = planerVelocity.x;
		enemy->character.velocity.z = planerVelocity.y;
	}

	enemy->character.velocity.y = enemy->character.physics.gravity * dt;

	OPvec3 vel = enemy->character.velocity + enemy->character.throwbackVelocity;

	OPphysXControllerMove(enemy->physXController, vel, timer);
	//
	OPvec3 pos = OPphysXControllerGetFootPos(enemy->physXController);
	enemy->character.position = pos;

	enemy->character.model.model.world.SetTranslate(enemy->character.position)->Scl(enemy->character.scale)->RotY(enemy->character.rotation.y);

	if (enemy->character.activeAnimation != &enemy->character.attack && enemy->character.activeAnimation != &enemy->character.special) {
		if (OPvec3Len(enemy->character.velocity) < 0.5) {
			enemy->character.activeAnimation = &enemy->character.idle;
		}
		else {
			enemy->character.activeAnimation = &enemy->character.walk;
		}
	}
}