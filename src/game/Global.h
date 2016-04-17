#pragma once

#include "OPengine.h"

extern OPcam CAMERA;
extern OPcam SHADOW_CAMERA;
extern OPcommandBucket RENDERBUCKET;
extern OPcommandBucket SHADOW_RENDERBUCKET;
extern OPeffect EFFECT;
extern OPeffect SHADOW_EFFECT;
extern OPmaterial MATERIAL;
extern OPmaterial SHADOW_MATERIAL;
extern OPvec3 LIGHTDIRECTION;
extern OPfontManager* FONTMANAGER;
extern OPtexture2D* UNAVILABLECARD;

void GlobalInit();