#pragma once

#include "./OPengine.h"
#include "../CardDesc.h"
#include "../Character.h"
#include "../Scene.h"

inline i8 CardWolfActivate(Character* player, Scene* scene) {
	if (player->energy >= 5) {
		player->energy -= 5;
		player->model.Init("wolf.opm", "wolf.png");
		player->damage = 10;
		player->defense = 1;
		player->speed = 3;
		player->form = 1;
		return 1;
	}
	return 0;
}

inline CardDesc CardWolf() {
	CardDesc result;
	result.Type = CardType::Wolf;
	result.Cost = 5;
	result.Form = 1;
	result.Shapeshift = 1;
	result.Texture = "WolfCard.png";
	result.Activate = CardWolfActivate;
	result.Available = CardIsAvailable;
	return result;
}