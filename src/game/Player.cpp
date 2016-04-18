#include "Player.h"
#include "Global.h"
#include "Scene.h"
#include "CardDesc.h"
#include "cards\wolf.h"
#include "cards\energy.h"
#include "cards\skeleton.h"
#include "cards\devilshowl.h"

void PlayerInit(Player * player, OPphysXControllerManager * manager, ui32 gamePad, const OPchar * model, const OPchar * texture)
{
	player->deck.Init(30);
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
	for (ui32 i = 0; i < 20; i++) {
		player->deck.AddCard(CardEnergy());
	}
	for (ui32 i = 0; i < 2; i++) {
		player->deck.AddCard(CardSkeleton());
	}
	for (ui32 i = 0; i < 2; i++) {
		player->deck.AddCard(CardWolf());
	}
	for (ui32 i = 0; i < 4; i++) {
		player->deck.AddCard(CardDevilsHowl());
	}
	player->deck.Shuffle();



	player->character.model.Init(model, texture);

	player->character.skeleton = *OPskeletonCopy((OPskeleton*)OPcmanLoadGet("ld35person.opm.skel"));
	player->character.idle = *(OPskeletonAnimation*)OPcmanLoadGet("ld35person.opm.Idle.anim");
	player->character.walk = *(OPskeletonAnimation*)OPcmanLoadGet("ld35person.opm.Walk.anim");
	player->character.attack = *(OPskeletonAnimation*)OPcmanLoadGet("ld35person.opm.Attack.anim");

	player->character.attack.Loop = 0;
	player->character.activeAnimation = &player->character.idle;



	player->material.Init(&EFFECT);
	player->material.AddParam("uColorTexture", player->character.model.texture);

	f32 x = gamePad * 200;
	player->character.position = OPvec3Create(-100.0 + x, 0, 0);
	player->character.rotation = OPVEC3_ZERO;
	player->character.scale = OPVEC3_ONE;
	player->character.radius = 5.0f;
	player->character.attackRange = 10.0f;
	player->character.damage = 1;
	player->character.health = 100;
	player->character.energy = 0;
	player->character.canAttack = 1;
	player->character.form = 0;
	player->character.speed = 1;
	player->character.defense = 1;
	player->character.knockback = 1.0;


	player->character.physics.maxSpeed = 2.0;
	player->character.physics.maxThrowbackSpeed = 10.0;
	player->character.physics.acceleration = 10.0;
	player->character.physics.drag = 8.0;
	player->character.physics.throwbackDrag = 12.0;
	player->character.physics.gravity = -9.8;
	player->character.physics.jumpSpeed = 1.22 * 4;

	player->character.velocity = OPVEC3_ZERO;
	player->character.throwbackVelocity = OPVEC3_ZERO;

	player->character.isAttacking = 0;
	player->character.attackTime = 0;
	player->character.maxAtackTime = 500;


	player->lastCardDrawn = 3000;


	OPphysXMaterial* material = OPphysXCreateMaterial(0.8, 0.8, 0.6);
	player->physXController = OPphysXControllerCreate(manager, material, 9, player->character.radius, NULL, NULL, NULL);
	OPphysXControllerSetPos(player->physXController, player->character.position);

	//OPphysXShape* triggerShape = OPphysXAddBoxShape(player->physXController->getActor(), material, OPvec3Create(6));
	////OPmat4 posM = OPmat4Translate(0, 0, 5);
	////PxTransform pos = OPphysXMat4ToPx(&posM);
	////triggerShape->setLocalPose(pos);
	//OPphysXSetSimulation(triggerShape, false);
	//OPphysXSetTrigger(triggerShape, true);
	//OPphysXSetSceneQuery(triggerShape, true);

	if (gamePad == 0) {
		player->controller = OPgamePadGet(OPGAMEPAD_ONE);
		player->keys[0] = OPKEY_A;
		player->keys[1] = OPKEY_D;
		player->keys[2] = OPKEY_W;
		player->keys[3] = OPKEY_S;
	}
	else {
		player->controller = OPgamePadGet(OPGAMEPAD_TWO);
		player->keys[0] = OPKEY_J;
		player->keys[1] = OPKEY_L;
		player->keys[2] = OPKEY_I;
		player->keys[3] = OPKEY_K;
	}
}

void PlayerUpdate(Player * player, Scene* scene, OPtimer * timer)
{	
	
	player->lastCardDrawn -= timer->Elapsed;
	if (player->lastCardDrawn <= 0 && player->deck.handCount < player->deck.handMax) {
		Card* card = player->deck.DrawCard();
		player->lastCardDrawn = 3000;
	}


	if (OPgamePadWasReleased(player->controller, OPgamePadButton::OPGAMEPADBUTTON_A) || OPkeyboardWasReleased(OPKEY_SPACE) || OPmouseWasPressed(OPMOUSE_RBUTTON)) {
		player->deck.ActivateCard(&player->character, scene);
	}

	if (OPgamePadWasReleased(player->controller, OPgamePadButton::OPGAMEPADBUTTON_Y) || OPkeyboardWasReleased(OPKEY_R)) {
		player->deck.KillCard();
	}

	if (OPgamePadRightThumbNowLeft(player->controller) || OPgamePadWasReleased(player->controller, OPgamePadButton::OPGAMEPADBUTTON_X) || OPkeyboardWasReleased(OPKEY_Q) || OPmouseWheelMoved() < 0) {
		player->deck.PrevCard();
	}

	if (OPgamePadRightThumbNowRight(player->controller) || OPgamePadWasReleased(player->controller, OPgamePadButton::OPGAMEPADBUTTON_B) || OPkeyboardWasReleased(OPKEY_E) || OPmouseWheelMoved() > 0) {
		player->deck.NextCard();
	}


	player->character.attackTime += timer->Elapsed;

	if (OPgamePadRightTriggerIsUp(player->controller)) {
		player->character.canAttack = 1;
	}

	if (player->character.attackTime > player->character.maxAtackTime) {
		player->character.activeAnimation = &player->character.walk;
	}

	if (player->character.canAttack && player->character.attackTime > player->character.maxAtackTime && (OPgamePadRightTrigger(player->controller) > 0.5f) || OPmouseWasPressed(OPMOUSE_LBUTTON)) {
		player->character.isAttacking = 1;
		player->character.attackTime = 0;
		player->character.canAttack = 0;
		player->character.attack.Frame = 0;
		player->character.attack.LoopsCompleted = 0;
		player->character.activeAnimation = &player->character.attack;
		OPfmodPlay(OPfmodLoad("Audio/woosh.wav"));
	}

	if (player->character.isAttacking && player->character.attackTime > player->character.maxAtackTime) {
		player->character.isAttacking = 0;
	}


	OPfloat dt = timer->Elapsed / 1000.0f;
	OPvec2 leftThumb = OPgamePadLeftThumb(player->controller);
	OPfloat speed = 1.0 + player->character.speed / 5.0;


	OPvec3 dir = OPVEC3_ZERO;

	OPvec3 forward = CAMERA.pos - CAMERA.target;
	forward.y = 0;
	forward = OPvec3Norm(forward);

	OPvec3 left = OPvec3Cross(forward, OPVEC3_UP);


	dir.x += leftThumb.x;
	dir.z += leftThumb.y;

	if (OPkeyboardIsDown(player->keys[0])) {
		dir.x -= 1.0f;
	}
	if (OPkeyboardIsDown(player->keys[1])) {
		dir.x += 1.0f;
	}
	if (OPkeyboardIsDown(player->keys[2])) {
		dir.z += 1.0f;
	}
	if (OPkeyboardIsDown(player->keys[3])) {
		dir.z -= 1.0f;
	}

	dir = OPvec3Norm(dir);

	OPvec2 planerVelocity = {
		player->character.velocity.x,
		player->character.velocity.z
	};
	OPvec2 velDragDir = OPvec2Norm(planerVelocity) * -player->character.physics.drag;

	if (player->character.velocity.x > 0) {
		player->character.velocity.x += velDragDir.x * dt;
		if (player->character.velocity.x < 0) player->character.velocity.x = 0;
	}
	else if (player->character.velocity.x < 0) {
		player->character.velocity.x += velDragDir.x * dt;
		if (player->character.velocity.x > 0) player->character.velocity.x = 0;
	}

	if (player->character.velocity.z > 0) {
		player->character.velocity.z += velDragDir.y * dt;
		if (player->character.velocity.z < 0) player->character.velocity.z = 0;
	}
	else if (player->character.velocity.z < 0) {
		player->character.velocity.z += velDragDir.y * dt;
		if (player->character.velocity.z > 0) player->character.velocity.z = 0;
	}


	OPvec2 throwbackPlanerVelocity = {
		player->character.throwbackVelocity .x,
		player->character.throwbackVelocity.z
	};
	if (OPvec2Len(throwbackPlanerVelocity) > player->character.physics.maxThrowbackSpeed) {
		throwbackPlanerVelocity = OPvec2Norm(throwbackPlanerVelocity) * player->character.physics.maxThrowbackSpeed;
		player->character.throwbackVelocity.x = throwbackPlanerVelocity.x;
		player->character.throwbackVelocity.z = throwbackPlanerVelocity.y;
	}
	OPvec2 throwbackVelDragDir = OPvec2Norm(throwbackPlanerVelocity) * -player->character.physics.throwbackDrag;

	if (player->character.throwbackVelocity.x > 0) {
		player->character.throwbackVelocity.x += throwbackVelDragDir.x * dt;
		if (player->character.throwbackVelocity.x < 0) player->character.throwbackVelocity.x = 0;
	}
	else if (player->character.throwbackVelocity.x < 0) {
		player->character.throwbackVelocity.x += throwbackVelDragDir.x * dt;
		if (player->character.throwbackVelocity.x > 0) player->character.throwbackVelocity.x = 0;
	}

	if (player->character.throwbackVelocity.z > 0) {
		player->character.throwbackVelocity.z += throwbackVelDragDir.y * dt;
		if (player->character.throwbackVelocity.z < 0) player->character.throwbackVelocity.z = 0;
	}
	else if (player->character.throwbackVelocity.z < 0) {
		player->character.throwbackVelocity.z += throwbackVelDragDir.y * dt;
		if (player->character.throwbackVelocity.z > 0) player->character.throwbackVelocity.z = 0;
	}

	OPfloat len = OPvec3Len(dir);
	if (len > 0) {

		OPvec3 acceleration = forward * -dir.z * player->character.physics.acceleration * speed;
		acceleration += left * -dir.x * player->character.physics.acceleration * speed;

		player->character.velocity += acceleration * dt;

		player->character.rotation = OPvec3Create(0, OPpi_2 + OPatan2(acceleration.x, -acceleration.z), 0);
	}

	player->character.velocity.y = player->character.physics.gravity * dt;
	planerVelocity.x = player->character.velocity.x;
	planerVelocity.y = player->character.velocity.z;

	if (OPvec2Len(planerVelocity) > player->character.physics.maxSpeed * speed) {
		planerVelocity = OPvec2Norm(planerVelocity) * player->character.physics.maxSpeed * speed;
		player->character.velocity.x = planerVelocity.x;
		player->character.velocity.z = planerVelocity.y;
	}

	OPvec3 vel = player->character.velocity + player->character.throwbackVelocity;

	OPphysXControllerMove(player->physXController, vel, timer);

	OPvec3 pos = OPphysXControllerGetFootPos(player->physXController);

	// OPvec3 oldPos = player->position;
	player->character.position = pos;

	if (player->character.activeAnimation != &player->character.attack && player->character.activeAnimation != &player->character.special) {
		if (OPvec3Len(player->character.velocity) < 0.5) {
			player->character.activeAnimation = &player->character.idle;
		}
		else {
			player->character.activeAnimation = &player->character.walk;
		}
	}


	//player->model.model.world.SetTranslate(player->position)->Scl(player->scale)->RotY(player->rotation.y);
}