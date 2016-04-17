#pragma once

#include "./OPengine.h"

#include "Character.h"
#include "Deck.h"

struct Scene;

struct Enemy;
typedef struct Enemy Enemy;

void EnemyInit(Enemy* enemy, OPphysXControllerManager* manager, const OPchar* model, const OPchar* texture);
void EnemyUpdate(Enemy* enemy, Scene* scene, OPtimer* timer);

struct Enemy {
	Deck deck;
	Character character;

	i64 lastCardDrawn = 1000;
	i64 lastMove = 1000;
	i64 actionTime = 1000;

	OPphysXController* physXController;

	void Init(OPphysXControllerManager* manager, const OPchar* model, const OPchar* texture) {
		EnemyInit(this, manager, model, texture);
	}

	void Update(OPtimer* timer, Scene* scene) {
		EnemyUpdate(this, scene, timer);
	}
};
