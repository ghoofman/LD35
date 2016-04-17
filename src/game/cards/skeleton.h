#pragma once

#include "./OPengine.h"
#include "../CardDesc.h"
#include "../Character.h"
#include "../Scene.h"

inline i8 CardSkeletonActivate(Character* player, Scene* scene) {
	if (player->energy >= 2) {
		player->energy -= 2;
		player->model.Init("skeleton.opm", "skeleton.png");
		player->damage = 3;
		player->defense = 1;
		player->speed = 1;
		player->knockback = 5;
		player->form = 2;
		return 1;
	}
	return 0;
}

inline CardDesc CardSkeleton() {
	CardDesc result;
	result.Type = CardType::Skeleton;
	result.Cost = 2;
	result.Form = 2;
	result.Shapeshift = 1;
	result.Texture = "Skel.png";
	result.Activate = CardSkeletonActivate;
	result.Available = CardIsAvailable;
	return result;
}