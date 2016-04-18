#pragma once

#include "./OPengine.h"
#include "../CardDesc.h"
#include "../Character.h"
#include "../Scene.h"

inline i8 CardSkeletonActivate(Character* player, Scene* scene) {
	if (player->energy >= 2) {
		player->energy -= 2;
		player->model.Init("ld35skeleton.opm", "skeleton.png");
		player->skeleton = *OPskeletonCopy((OPskeleton*)OPcmanLoadGet("ld35skeleton.opm.skel"));
		player->idle = *(OPskeletonAnimation*)OPcmanLoadGet("ld35skeleton.opm.Idle.anim");
		player->walk = *(OPskeletonAnimation*)OPcmanLoadGet("ld35person.opm.Walk.anim");
		player->attack = *(OPskeletonAnimation*)OPcmanLoadGet("ld35person.opm.Attack.anim");
		//player->special = *(OPskeletonAnimation*)OPcmanLoadGet("ld35wolf.opm.Howl.anim");
		player->activeAnimation = &player->idle;
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