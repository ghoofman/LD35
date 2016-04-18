#include "Scene.h"

// Manage all of the entities
// This is the starting point of a level

void OnContact(OPphysXContact contact) {
	OPlog("Contact was made");
}
void OnTigger(OPphysXTrigger trigger) {
	OPlog("Trigger was made");
}

void SceneInit(Scene* result) {

	// OPsprite2DInit(NULL);

	result->physXScene = OPphysXSceneCreate(OPvec3Create(0, -9.8, 0), OnTigger, OnContact);
	result->physXManager = OPphysXControllerCreateManager(result->physXScene);
	OPphysXDebugger("127.0.0.1");

	result->winner = 0;

	result->staticEntitiesCount = 0;
	result->staticEntitiesMax = 16;
	result->staticEntities = (OPmodelTextured*)OPalloc(sizeof(OPmodelTextured) * result->staticEntitiesMax);

	result->enemyCount = 0;
	result->enemyMax = 16;
	result->enemies = (Enemy*)OPalloc(sizeof(Enemy) * result->enemyMax);

	result->playerCount = 0;


	{
		PxTransform transform = PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
		OPphysXMaterial* material = OPphysXCreateMaterial(0.8, 0.8, 0.6);
		OPphysXRigidStatic* plane = OPphysXSceneCreateStatic(result->physXScene, transform);
		OPphysXAddPlaneShape(plane, material);
		OPphysXSceneAddActor(result->physXScene, plane);
	}

	{
		PxTransform transform = PxTransform(PxVec3(-250.0f, 0.0f, 0.0f), PxQuat(0, PxVec3(1.0f, 0.0f, 0.0f)));
		OPphysXMaterial* material = OPphysXCreateMaterial(0.8, 0.8, 0.6);
		OPphysXRigidStatic* plane = OPphysXSceneCreateStatic(result->physXScene, transform);
		OPphysXAddPlaneShape(plane, material);
		OPphysXSceneAddActor(result->physXScene, plane);
	}

	{
		PxTransform transform = PxTransform(PxVec3(250.0f, 0.0f, 0.0f), PxQuat(PxPi, PxVec3(0.0f, 1.0f, 0.0f)));
		OPphysXMaterial* material = OPphysXCreateMaterial(0.8, 0.8, 0.6);
		OPphysXRigidStatic* plane = OPphysXSceneCreateStatic(result->physXScene, transform);
		OPphysXAddPlaneShape(plane, material);
		OPphysXSceneAddActor(result->physXScene, plane);
	}

	{
		PxTransform transform = PxTransform(PxVec3(0.0f, 0.0f, 250.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 1.0f, 0.0f)));
		OPphysXMaterial* material = OPphysXCreateMaterial(0.8, 0.8, 0.6);
		OPphysXRigidStatic* plane = OPphysXSceneCreateStatic(result->physXScene, transform);
		OPphysXAddPlaneShape(plane, material);
		OPphysXSceneAddActor(result->physXScene, plane);
	}

	{
		PxTransform transform = PxTransform(PxVec3(0.0f, 0.0f, -250.0f), PxQuat(-PxHalfPi, PxVec3(0.0f, 1.0f, 0.0f)));
		OPphysXMaterial* material = OPphysXCreateMaterial(0.8, 0.8, 0.6);
		OPphysXRigidStatic* plane = OPphysXSceneCreateStatic(result->physXScene, transform);
		OPphysXAddPlaneShape(plane, material);
		OPphysXSceneAddActor(result->physXScene, plane);
	}


	//result->p1gui = OPtexture2DCreate((OPtexture*)OPcmanLoadGet("P1GUI.png"));
	//result->p2gui = OPtexture2DCreate((OPtexture*)OPcmanLoadGet("P2GUI.png"));
	//result->p1gui->Scale = result->p2gui->Scale = OPVEC2_ONE * 0.1;
	//result->p1gui->Position.y = result->p2gui->Position.y = 0.8;
	//result->p1gui->Position.x = -0.5;
	//result->p2gui->Position.x = 0.5;

	OPcmanLoad("gui.opss");
	result->sprites[0] = (OPsprite*)OPcmanGet("gui/P1GUI");
	result->sprites[1] = (OPsprite*)OPcmanGet("gui/P2GUI");
	result->sprites[2] = (OPsprite*)OPcmanGet("gui/GUIHealth");
	result->sprites[3] = (OPsprite*)OPcmanGet("gui/GUIGuard");
	result->sprites[4] = (OPsprite*)OPcmanGet("gui/GUIEnergy");
	result->spritesBars[0] = (OPsprite*)OPcmanGet("gui/GUIHealth");
	result->spritesBars[1] = (OPsprite*)OPcmanGet("gui/GUIGuard");
	result->spritesBars[2] = (OPsprite*)OPcmanGet("gui/GUIEnergy");
	OPspriteSystemEffectDefault(&result->spriteEffect);
	OPspriteSystemInit(&result->spriteSystem, result->sprites, 5, &result->spriteEffect, OPSPRITESYSTEMALIGN_BOTTOM_LEFT);
	OPspriteSystemInit(&result->spriteSystemBars, result->spritesBars, 5, &result->spriteEffect, OPSPRITESYSTEMALIGN_BOTTOM_CENTER);

	OPspriteSystemSprite* p1GUI = OPspriteSystemAdd(&result->spriteSystem);
	p1GUI->Scale = OPvec2Create(1, 1);
	p1GUI->Position = OPvec2Create(50, OPRENDER_HEIGHT - 120);

	result->p1Energy = OPspriteSystemAdd(&result->spriteSystemBars);
	result->p1Energy->CurrentSprite = 2;
	f32 health = 0.0f;
	result->p1Energy->Scale = OPvec2Create(health / 2.0, 1);
	result->p1Energy->Position = OPvec2Create(95 + health, OPRENDER_HEIGHT - 120);

	result->p1Health = OPspriteSystemAdd(&result->spriteSystemBars);
	result->p1Health->CurrentSprite = 0;
	health = 100;
	result->p1Health->Scale = OPvec2Create(health / 2.0, 1);
	result->p1Health->Position = OPvec2Create(55 + health, OPRENDER_HEIGHT - 120);



	OPspriteSystemSprite* p2GUI = OPspriteSystemAdd(&result->spriteSystem);
	p2GUI->CurrentSprite = 1;
	p2GUI->Scale = OPvec2Create(1, 1);
	p2GUI->Position = OPvec2Create(OPRENDER_WIDTH - 500, OPRENDER_HEIGHT - 120);



	result->p2Energy = OPspriteSystemAdd(&result->spriteSystemBars);
	result->p2Energy->CurrentSprite = 2;
	health = 0.0f;
	result->p2Energy->Scale = OPvec2Create(health / 2.0, 1);
	result->p2Energy->Position = OPvec2Create(OPRENDER_WIDTH - 50 - health, OPRENDER_HEIGHT - 120);

	result->p2Health = OPspriteSystemAdd(&result->spriteSystemBars);
	result->p2Health->CurrentSprite = 0;
	health = 100;
	result->p2Health->Scale = OPvec2Create(health / 2.0, 1);
	result->p2Health->Position = OPvec2Create(OPRENDER_WIDTH - 500 + health, OPRENDER_HEIGHT - 120);



	f32 healthPercent = (25 / 100.0f);
	if (healthPercent > 1.0f) healthPercent = 1.0f;
	health = healthPercent * 222.0f;
	result->p2Health->Scale = OPvec2Create(health / 2.0, 1);
	result->p2Health->Position = OPvec2Create(OPRENDER_WIDTH - 50 - health, OPRENDER_HEIGHT - 120);




	result->camera = OPcamOrtho(OPvec3Create(0, 0, 10), OPVEC3_ZERO, OPVEC3_UP, 0.1f, 20.0f, 0, (OPfloat)OPRENDER_WIDTH, 0, (OPfloat)OPRENDER_HEIGHT);

}

Scene* SceneCreate() {
	Scene* result = (Scene*)OPalloc(sizeof(Scene));
	SceneInit(result);
	return result;
}

Player* SceneAddPlayer(Scene* scene, ui32 gamepad, const OPchar* model, const OPchar* texture) {
	ASSERT(scene->playerCount < MAX_PLAYERS - 1, "Already at max players, can't add another");
	scene->players[scene->playerCount].Init(scene->physXManager, gamepad, model, texture);
	return &scene->players[scene->playerCount++];
}

Enemy* SceneAddEnemy(Scene* scene, const OPchar* model, const OPchar* texture) {
	ASSERT(scene->enemyCount < scene->enemyMax - 1, "Already at max enemies, can't add another");
	scene->enemies[scene->enemyCount].Init(scene->physXManager, model, texture);
	return &scene->enemies[scene->enemyCount++];
}

// Returns the next available OPmodelTextured*
OPmodelTextured* SceneAddStatic(Scene* scene, const OPchar* model, const OPchar* texture) {
	ASSERT(scene->staticEntitiesCount < scene->staticEntitiesMax - 1, "Filled up Static Entities");
	scene->staticEntities[scene->staticEntitiesCount].Init(model, texture);
	return &scene->staticEntities[scene->staticEntitiesCount++];
}

void SceneUpdate(Scene* scene, OPtimer* timer) {
	OPspriteSystemUpdate(&scene->spriteSystem, timer);
	OPspriteSystemUpdate(&scene->spriteSystemBars, timer);

	f32 energyPercent = (scene->players[0].character.energy / 9.0f);
	if (energyPercent > 1.0f) energyPercent = 1.0f;
	f32 energy = energyPercent * 92.0f;
	scene->p1Energy->Scale = OPvec2Create(energy / 2.0, 1);
	scene->p1Energy->Position = OPvec2Create(95 + energy, OPRENDER_HEIGHT - 120);

	f32 healthPercent = (scene->players[0].character.health / 100.0f);
	if (healthPercent > 1.0f) healthPercent = 1.0f;
	f32 health = healthPercent * 222.0f;
	scene->p1Health->Scale = OPvec2Create(health / 2.0, 1);
	scene->p1Health->Position = OPvec2Create(57 + health, OPRENDER_HEIGHT - 120);



	healthPercent = (scene->enemies[0].character.health / 100.0f);
	if (healthPercent > 1.0f) healthPercent = 1.0f;
	health = healthPercent * 222.0f;
	scene->p2Health->Scale = OPvec2Create(health / 2.0, 1);
	scene->p2Health->Position = OPvec2Create(OPRENDER_WIDTH - 50 - health, OPRENDER_HEIGHT - 120);


	energyPercent = (scene->enemies[0].character.energy / 9.0f);
	if (energyPercent > 1.0f) energyPercent = 1.0f;
	energy = energyPercent * 92.0f;
	scene->p2Energy->Scale = OPvec2Create(energy / 2.0, 1);
	scene->p2Energy->Position = OPvec2Create(OPRENDER_WIDTH - 88 - energy, OPRENDER_HEIGHT - 120);


	//f32 energyPercent = (scene->players[0].character.energy / 9.0f);
	//if (energyPercent > 1.0f) energyPercent = 1.0f;
	//f32 energy = (scene->players[0].character.energy / 9.0f) * 92.0f;
	//scene->p1Energy->Scale = OPvec2Create(energy / 2.0, 1);
	//scene->p1Energy->Position = OPvec2Create(95 + energy, OPRENDER_HEIGHT - 120);


	OPphysXSceneUpdate(scene->physXScene, timer);

	scene->enemies[0].character.target = scene->players[0].character.position;
	
	// Update the physics of each player
	for (ui32 i = 0; i < scene->playerCount; i++) {
		scene->players[i].Update(timer, scene);

		if (scene->players[i].character.activeAnimation != NULL) {

			OPskeletonAnimationUpdate(scene->players[i].character.activeAnimation, timer);
			OPskeletonAnimationApply(scene->players[i].character.activeAnimation, &scene->players[i].character.skeleton);
			OPskeletonUpdate(&scene->players[i].character.skeleton);
		}
	}

	for (ui32 i = 0; i < scene->enemyCount; i++) {
		if (scene->enemies[i].character.health <= 0) {
			scene->enemyCount--;
			break;
		}

		if (scene->enemies[i].character.activeAnimation != NULL) {

			OPskeletonAnimationUpdate(scene->enemies[i].character.activeAnimation, timer);
			OPskeletonAnimationApply(scene->enemies[i].character.activeAnimation, &scene->enemies[i].character.skeleton);
			OPskeletonUpdate(&scene->enemies[i].character.skeleton);
		}
		scene->enemies[i].Update(timer, scene);
	}

	// Check for hits
	for (ui32 i = 0; i < scene->playerCount; i++) {
		if (scene->players[i].character.isAttacking) {
			for (ui32 j = 0; j < scene->enemyCount; j++) {
				OPvec3 relPos = scene->enemies[j].character.position - scene->players[i].character.position;

				OPfloat radSumSqr = scene->players[i].character.radius + scene->enemies[j].character.radius;

				OPfloat distSqr = OPvec3Dot(relPos, relPos);

				f32 range = radSumSqr * radSumSqr;
				f32 attackRange = radSumSqr * scene->players[i].character.attackRange * 4;

				if (distSqr <= attackRange) {
					// We're within 10 units of an enemy, and we're attacking.
					// TODO: (garrett) Check if facing
					OPfmodPlay(OPfmodLoad("Audio/hit.wav"));
					scene->enemies[j].character.health -= scene->players[i].character.damage;

					OPvec3 rel = OPvec3Norm(relPos);
					scene->enemies[j].character.throwbackVelocity += rel * scene->players[i].character.knockback;

					if (scene->enemies[j].character.health <= 0) {
						scene->enemyCount--;
					}
					scene->players[i].character.isAttacking = 0;
					break;
				}
			}

			// In the case that it's player vs player
			for (ui32 j = 0; j < scene->playerCount; j++) {
				if (j == i) continue;

				OPvec3 relPos = scene->players[j].character.position - scene->players[i].character.position;

				OPfloat radSumSqr = scene->players[i].character.radius + scene->players[j].character.radius;

				OPfloat distSqr = OPvec3Dot(relPos, relPos);

				f32 range = radSumSqr * radSumSqr;
				f32 attackRange = radSumSqr * scene->players[i].character.attackRange * 4;

				if (distSqr <= attackRange) {
					// We're within 10 units of an enemy, and we're attacking.
					// TODO: (garrett) Check if facing
					OPfmodPlay(OPfmodLoad("Audio/hit.wav"));
					scene->players[j].character.health -= scene->players[i].character.damage;

					OPvec3 rel = OPvec3Norm(relPos);
					scene->players[j].character.throwbackVelocity += rel * scene->players[i].character.knockback;

					if (scene->players[j].character.health <= 0) {
						scene->playerCount--;
					}
					scene->players[i].character.isAttacking = 0;
					break;
				}
			}
		}
	}

	for (ui32 i = 0; i < scene->enemyCount; i++) {
		if (scene->enemies[i].character.isAttacking) {
			for (ui32 j = 0; j < scene->playerCount; j++) {
				OPvec3 relPos = scene->players[j].character.position - scene->enemies[i].character.position;

				OPfloat radSumSqr = scene->enemies[i].character.radius + scene->players[j].character.radius;

				OPfloat distSqr = OPvec3Dot(relPos, relPos);

				f32 range = radSumSqr * radSumSqr;
				f32 attackRange = radSumSqr * scene->enemies[i].character.attackRange * 4;

				if (distSqr <= attackRange) {
					// We're within 10 units of an enemy, and we're attacking.
					// TODO: (garrett) Check if facing
					OPfmodPlay(OPfmodLoad("Audio/hit.wav"));
					scene->players[j].character.health -= scene->enemies[i].character.damage;

					OPvec3 rel = OPvec3Norm(relPos);
					scene->players[j].character.throwbackVelocity += rel * scene->enemies[i].character.knockback;

					if (scene->players[j].character.health <= 0) {
						scene->playerCount--;
					}
					scene->enemies[i].character.isAttacking = 0;
					break;
				}
			}
		}
	}

	if (scene->playerCount + scene->enemyCount <= 1) {
		// Only 1 player left, declare the winner
		if (scene->enemyCount > scene->playerCount) {
			scene->winner = 2;
			OPfmodPlay(OPfmodLoad("Audio/lost.wav"));
		}
		else if (scene->playerCount > scene->enemyCount) {
			scene->winner = 1;
			OPfmodPlay(OPfmodLoad("Audio/win.wav"));
		}
	}



	CAMERA.target = scene->players[0].character.position;
	//CAMERA.pos = scene->players[0].position;
	//CAMERA.pos += OPvec3Create(0, 1000, 1000);
	CAMERA.Update();


}

void Scene::RenderShadows(OPfloat delta) {
	//for (ui32 i = 0; i < playerCount; i++) {
	//	players[i].UpdateWorld(delta);
	//	RENDERBUCKET.CreateDrawIndexedSubmit(
	//		&players[i].model, &SHADOW_MATERIAL);
	//}

	//for (ui32 i = 0; i < enemyCount; i++) {
	//	RENDERBUCKET.CreateDrawIndexedSubmit(
	//		&enemies[i].model, &SHADOW_MATERIAL);
	//}

	//RENDERBUCKET.Render();
}

void Scene::Render(OPfloat delta) {
	char buff[32];

	for (ui32 i = 0; i < staticEntitiesCount; i++) {
		RENDERBUCKET.CreateDrawIndexedSubmit(&staticEntities[i], &MATERIAL);
	}

	RENDERBUCKET.Render();

	for (ui32 i = 0; i < playerCount; i++) {
		players[i].UpdateWorld(delta);
		if (players[i].character.activeAnimation != NULL) {

			OPmeshBind(players[i].character.model.model.mesh);
			OPeffectBind(&SKINNING_EFFECT);

			//OPmat4 world;
			//OPmat4Identity(&world);
			//OPmat4BuildRotX(&world,- OPpi / 2.0);

			OPeffectParamMat4("uWorld", &players[i].character.model.model.world);
			OPeffectParamMat4("uView", &CAMERA.view);
			OPeffectParamMat4("uProj", &CAMERA.proj);

			OPeffectParamMat4v("uBones", players[i].character.skeleton.hierarchyCount, players[i].character.skeleton.skinned);

			OPvec3 light = OPvec3Create(0, 10, 0);
			OPeffectParamVec3("uLightPosition", &light);

			OPtextureClearActive();
			OPeffectParami("uColorTexture", OPtextureBind(players[i].character.model.texture));

			OPmeshRender();
		}
		//RENDERBUCKET.CreateDrawIndexedSubmit(&players[i].character.model, &MATERIAL);
	}

	for (ui32 i = 0; i < enemyCount; i++) {
		if (enemies[i].character.activeAnimation != NULL) {

			OPmeshBind(enemies[i].character.model.model.mesh);
			OPeffectBind(&SKINNING_EFFECT);

			//OPmat4 world;
			//OPmat4Identity(&world);
			//OPmat4BuildRotX(&world,- OPpi / 2.0);

			OPeffectParamMat4("uWorld", &enemies[i].character.model.model.world);
			OPeffectParamMat4("uView", &CAMERA.view);
			OPeffectParamMat4("uProj", &CAMERA.proj);

			OPeffectParamMat4v("uBones", enemies[i].character.skeleton.hierarchyCount, enemies[i].character.skeleton.skinned);

			OPvec3 light = OPvec3Create(0, 10, 0);
			OPeffectParamVec3("uLightPosition", &light);

			OPtextureClearActive();
			OPeffectParami("uColorTexture", OPtextureBind(enemies[i].character.model.texture));

			OPmeshRender();
		}
		//RENDERBUCKET.CreateDrawIndexedSubmit(&enemies[i].character.model, &MATERIAL);
	}

	i8 dir = 1;
	for (ui32 j = 0; j < playerCount; j++) {
		f32 offset = j;
		for (ui32 i = 0; i < players[j].deck.handCount; i++) {
			players[j].deck.hand[i].Render(&players[0].character, this, offset, players[j].deck.activeCard == i, dir);
			offset += 0.12 + (0.075 * (players[j].deck.activeCard == i));
		}
		dir *= -1;
	}

	for (ui32 j = 0; j < enemyCount; j++) {
		f32 offset = j;
		for (ui32 i = 0; i < enemies[j].deck.handCount; i++) {
			enemies[j].deck.hand[i].Render(&enemies[0].character, this, offset, enemies[j].deck.activeCard == i, -1);
			offset += 0.12 + (0.075 * (enemies[j].deck.activeCard == i));
		}
	}

	OPrenderDepth(0);
	OPtexturePixelate();
	OPspriteSystemRender(&spriteSystemBars, &camera);
	OPspriteSystemRender(&spriteSystem, &camera);

	OPfontRenderBegin(FONTMANAGER);
	for (ui32 i = 0; i < playerCount; i++) {
		if (i == 1) {

			itoa(players[i].character.energy, buff, 10);
			OPfontManagerSetAlign(OPfontAlign::OPFONT_ALIGN_CENTER);
			FONTMANAGER->scale = 0.3;
			OPfontManagerSetColor(OPvec3Create(1, 1, 0));
			OPfontRender(buff, OPvec2Create(OPRENDER_SCREEN_WIDTH - 57, 85));
			OPfontManagerSetColor(OPvec3Create(1, 1, 1));
			FONTMANAGER->scale = 1.0;
		}
		else {

			itoa(players[i].character.energy, buff, 10);
			OPfontManagerSetAlign(OPfontAlign::OPFONT_ALIGN_CENTER);
			FONTMANAGER->scale = 0.3;
			OPfontManagerSetColor(OPvec3Create(1, 1, 0));
			OPfontRender(buff, OPvec2Create(85, 85));
			OPfontManagerSetColor(OPvec3Create(1, 1, 1));
			FONTMANAGER->scale = 1.0;

		}
	}
	OPfontManagerSetAlign(OPfontAlign::OPFONT_ALIGN_RIGHT);
	for (ui32 i = 0; i < enemyCount; i++) {

		itoa(enemies[i].character.energy, buff, 10);
		OPfontManagerSetAlign(OPfontAlign::OPFONT_ALIGN_CENTER);
		FONTMANAGER->scale = 0.3;
		OPfontManagerSetColor(OPvec3Create(1, 1, 0));
		OPfontRender(buff, OPvec2Create(OPRENDER_SCREEN_WIDTH - 57, 85));
		OPfontManagerSetColor(OPvec3Create(1, 1, 1));
		FONTMANAGER->scale = 1.0;
	}

	OPfontRenderEnd();
}
