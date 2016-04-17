#pragma once

#include "OPengine.h"

#include "Character.h"
#include "Deck.h"

struct Scene;

struct Player;
typedef struct Player Player;

void PlayerInit(Player* player, OPphysXControllerManager* manager, ui32 gamePad, const OPchar* model, const OPchar* texture);
void PlayerUpdate(Player* player, Scene* scene, OPtimer* timer);

struct Player {
	OPgamePad* controller;
	Character character;
	Deck deck;
	OPmaterial material;

	i64 lastCardDrawn = 1000;

	enum OPkeyboardKey keys[4];

	OPphysXController* physXController;

	void Init(OPphysXControllerManager* manager, ui32 gamePad, const OPchar* model, const OPchar* texture) {
		PlayerInit(this, manager, gamePad, model, texture);
	}

	void Update(OPtimer* timer, Scene* scene) {
		PlayerUpdate(this, scene, timer);
	}

	void UpdateWorld(OPfloat delta) {
		character.model.model.world.SetTranslate(character.position)->Scl(character.scale)->RotY(character.rotation.y);
	}

	void ShapeshiftWolf() {
		character.model.Init("wolf.opm", "wolf.png");
		character.damage = 10;
		character.form = 1;
	}

	void ShapeshiftSkeleton() {
		character.model.Init("skeleton.opm", "skeleton.png");
		character.damage = 5;
		character.form = 2;
	}
};