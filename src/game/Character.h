#pragma once

#include "./OPengine.h"
#include "Physics.h"

struct Character;
typedef struct Character Character;

struct Character {
	OPmodelTextured model;
	OPvec3 position;
	OPvec3 rotation;
	OPvec3 scale;
	OPfloat radius;
	OPfloat attackRange;
	i32 health;
	ui32 energy;
	ui32 damage;

	f32 defense;
	f32 speed;
	f32 knockback;

	OPvec3 target;
	ui64 form;


	i8 isAttacking;
	i8 canAttack;
	ui64 attackTime;
	ui64 maxAtackTime;

	OPvec3 velocity;
	OPvec3 acceleration;
	EntityPhysics physics;
	OPvec3 throwbackVelocity;
};
