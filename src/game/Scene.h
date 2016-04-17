#pragma once

#include "./OPengine.h"

#include "Global.h"
#include "Player.h"
#include "Enemy.h"
#include "Card.h"

struct Scene;
typedef struct Scene Scene;

void SceneInit(Scene* result);
Scene* SceneCreate();
void SceneUpdate(Scene* scene, OPtimer* timer);
Player* SceneAddPlayer(Scene* scene, ui32 gamepad, const OPchar* model, const OPchar* texture);
Enemy* SceneAddEnemy(Scene* scene, const OPchar* model, const OPchar* texture);
OPmodelTextured* SceneAddStatic(Scene* scene, const OPchar* model, const OPchar* texture);

#define MAX_PLAYERS 4

struct Scene {
	OPphysXScene* physXScene;
	OPphysXControllerManager* physXManager;

	ui32 staticEntitiesCount;
	ui32 staticEntitiesMax;
	OPmodelTextured* staticEntities;


	ui32 playerCount;
	Player players[MAX_PLAYERS];

	ui32 enemyCount;
	ui32 enemyMax;
	Enemy* enemies;


	OPspriteSystem spriteSystem;
	OPspriteSystem spriteSystemBars;
	OPeffect spriteEffect;
	OPsprite* sprites[5];
	OPsprite* spritesBars[3];
	OPsprite* p1guiSprite, *p2guiSprite;
	OPsprite2D* p1gui;
	OPsprite2D* p2gui;
	OPspriteSystemSprite* p1Energy;
	OPspriteSystemSprite* p1Health;
	OPspriteSystemSprite* p1Guard;
	OPspriteSystemSprite* p2Energy;
	OPspriteSystemSprite* p2Health;
	OPspriteSystemSprite* p2Guard;
	OPcam camera;

	void Init() {
		SceneInit(this);
	}

	void Update(OPtimer* timer) {
		SceneUpdate(this, timer);
	}

	Player* AddPlayer(ui32 gamepad, const OPchar* model, const OPchar* texture) {
		return SceneAddPlayer(this, gamepad, model, texture);
	}

	Enemy* AddEnemy(const OPchar* model, const OPchar* texture) {
		return SceneAddEnemy(this, model, texture);
	}

	OPmodelTextured* AddStatic(const OPchar* model, const OPchar* texture) {
		return SceneAddStatic(this, model, texture);
	}

	void RenderShadows(OPfloat delta);

	void Render(OPfloat delta);
};