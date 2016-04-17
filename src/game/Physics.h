#pragma once

#include "./OPengine.h"

struct EntityPhysics;
typedef struct EntityPhysics EntityPhysics;

struct EntityPhysics {
	OPfloat maxSpeed;
	OPfloat maxThrowbackSpeed;
	OPfloat acceleration;
	OPfloat drag;
	OPfloat throwbackDrag;
	OPfloat gravity;
	OPfloat jumpSpeed;
};
