#pragma once

#include "OPengine.h"
#include "Character.h"
#include "cards/CardType.h"

struct Scene;

struct CardDesc;
typedef struct CardDesc CardDesc;

struct CardDesc {
	CardType Type;
	ui8 Cost;
	ui8 Form;
	ui8 Shapeshift;
	const OPchar* Texture;
	i8(*Activate)(Character*, Scene*);
	i8(*Available)(Character*, Scene*);
};
