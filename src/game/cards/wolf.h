#pragma once

#include "./OPengine.h"
#include "../CardDesc.h"
#include "../Character.h"
#include "../Scene.h"

inline i8 CardWolfActivate(Character* player, Scene* scene) {
	if (player->energy >= 5) {
		player->energy -= 5;
		player->model.Init("ld35wolf.opm", "wolf.png");
		player->skeleton = *OPskeletonCopy((OPskeleton*)OPcmanLoadGet("ld35wolf.opm.skel"));
		player->idle = *(OPskeletonAnimation*)OPcmanLoadGet("ld35wolf.opm.Idle.anim");
		player->walk = *(OPskeletonAnimation*)OPcmanLoadGet("ld35wolf.opm.Walk.anim");
		player->attack = *(OPskeletonAnimation*)OPcmanLoadGet("ld35wolf.opm.Attack.anim");
		player->special = *(OPskeletonAnimation*)OPcmanLoadGet("ld35wolf.opm.Howl.anim");
		player->special.Loop = 0;

		player->activeAnimation = &player->idle;
		player->damage = 10;
		player->defense = 1;
		player->speed = 3 * 2;
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