#pragma once

#include "./OPengine.h"
#include "../CardDesc.h"
#include "../Character.h"
#include "../Scene.h"

inline i8 CardDevilsHowlActivate(Character* player, Scene* scene) {
	if (player->energy >= 4) {
		player->energy -= 4;
		player->activeAnimation = &player->special;
		OPfmodPlay(OPfmodLoad("Audio/howl2.wav"));
		for (ui32 i = 0; i < scene->enemyCount; i++) {
			scene->enemies[i].character.health -= 10;
			OPvec3 rel = player->position - scene->enemies[i].character.position;
			rel = OPvec3Norm(rel);
			scene->enemies[i].character.throwbackVelocity -= rel * 1000;
		}
		return 1;
	}
	return 0;
}

inline i8 CardDevilsHowlIsAvailable(Character* player, Scene* scene) {
	return player->form == 1;
}

inline CardDesc CardDevilsHowl() {
	CardDesc result;
	result.Type = CardType::DevilsHowl;
	result.Cost = 4;
	result.Form = 0;
	result.Shapeshift = 0;
	result.Texture = "DevilsHowlCard.png";
	result.Activate = CardDevilsHowlActivate;
	result.Available = CardDevilsHowlIsAvailable;
	return result;
}