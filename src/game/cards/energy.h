#pragma once

#include "./OPengine.h"
#include "../CardDesc.h"
#include "../Character.h"
#include "../Scene.h"

inline i8 CardEnergyActivate(Character* player, Scene* scene) {
	player->energy++;
	return 1;
}

inline CardDesc CardEnergy() {
	CardDesc result;
	result.Type = CardType::Energy;
	result.Cost = 0;
	result.Form = 0;
	result.Shapeshift = 0;
	result.Texture = "Energy.png";
	result.Activate = CardEnergyActivate;
	result.Available = CardIsAvailable;
	return result;
}