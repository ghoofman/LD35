#include "Global.h"

OPcam CAMERA;
OPcam SHADOW_CAMERA;
OPcommandBucket RENDERBUCKET;
OPcommandBucket SHADOW_RENDERBUCKET;
OPeffect EFFECT;
OPeffect SHADOW_EFFECT;
OPeffect SKINNING_EFFECT;
OPmaterial MATERIAL;
OPmaterial SHADOW_MATERIAL;
OPvec3 LIGHTDIRECTION;
OPfontManager* FONTMANAGER;
OPtexture2D* UNAVILABLECARD;

OPshaderAttribute TEXTURED_VERTEX_LAYOUT_ATTRIBUTES[3] = {
	{ "aPosition", GL_FLOAT, 3, 0 },
	{ "aNormal", GL_FLOAT, 3, 12 },
	{ "aUV", GL_FLOAT, 2, 24 }
};
OPvertexLayout TEXTURED_VERTEX_LAYOUT;


OPshaderAttribute SHADOW_VERTEX_LAYOUT_ATTRIBUTES[1] = {
	{ "aPosition", GL_FLOAT, 3, 0 }
};
OPvertexLayout SHADOW_VERTEX_LAYOUT;

OPshaderAttribute SKINNING_VERTEX_LAYOUT_ATTRIBUTES[] = {
	{ "aPosition", GL_FLOAT, 3 },
	{ "aNormal", GL_FLOAT, 3 },
	//{ "aTangent", GL_FLOAT, 3 },
	{ "aUV", GL_FLOAT, 2 },
	{ "aBlendIndices", GL_FLOAT, 4 },
	{ "aBlendWeights", GL_FLOAT, 4 }
};
OPvertexLayout SKINNING_VERTEX_LAYOUT;

i8 GLOBAL_LOADED = 0;

void GlobalInit()
{
	if (GLOBAL_LOADED) return;
	GLOBAL_LOADED = 1;

	UNAVILABLECARD = OPtexture2DCreate((OPtexture*)OPcmanLoadGet("Unavailable.png"));

	CAMERA.SetPerspective(OPvec3Create(0, 200, 320), OPVEC3_UP);

	OPfloat size = 100;
	SHADOW_CAMERA = OPcamOrtho(
		OPvec3Create(200, 200, 200),
		OPvec3Create(0, 0, 0),
		OPvec3Create(0, 1, 0),
		0.1f, 1000.0f,
		-size, size,
		-size, size);

	//SHADOW_CAMERA = OPcamPersp(
	//	OPvec3Create(200, 200, 200),
	//	OPvec3Create(0, 0, 0),
	//	OPvec3Create(0, 1, 0),
	//	1.0f, 1000.0f,
	//	45.0f, 1.0
	//);


	RENDERBUCKET.Init(16, &CAMERA);
	SHADOW_RENDERBUCKET.Init(16, &SHADOW_CAMERA);

	LIGHTDIRECTION = OPVEC3_UP;

	OPvertexLayoutInit(&TEXTURED_VERTEX_LAYOUT, TEXTURED_VERTEX_LAYOUT_ATTRIBUTES, 3);

	OPvertexLayoutInit(&SHADOW_VERTEX_LAYOUT, SHADOW_VERTEX_LAYOUT_ATTRIBUTES, 1);

	OPvertexLayoutInit(&SKINNING_VERTEX_LAYOUT, SKINNING_VERTEX_LAYOUT_ATTRIBUTES, 5);


	EFFECT.Init("Common/TexturedShadow.vert", "Common/TexturedShadow.frag", &TEXTURED_VERTEX_LAYOUT);
	MATERIAL.Init(&EFFECT);
	//OPmaterialAddParam(&MATERIAL, "uViewShadow", &SHADOW_CAMERA.view);
	//OPmaterialAddParam(&MATERIAL, "uProjShadow", &SHADOW_CAMERA.proj);
	OPmaterialAddParam(&MATERIAL, "uLightPos", &SHADOW_CAMERA.pos);
	OPmaterialAddParam(&MATERIAL, "uViewPos", &CAMERA.pos);



	OPshaderAttribute attribsShadow[] = {
		{ "aPosition", GL_FLOAT, 3 }
	};

	OPshader* vert = (OPshader*)OPcmanLoadGet("Common/DepthRTT.vert");
	OPshader* frag = (OPshader*)OPcmanLoadGet("Common/DepthRTT.frag");
	SHADOW_EFFECT = OPeffectCreate(
		*vert,
		*frag,
		attribsShadow,
		1,
		"Shadow Effect",
		TEXTURED_VERTEX_LAYOUT.stride
	);

	SHADOW_MATERIAL.Init(&SHADOW_EFFECT);


	FONTMANAGER = OPfontManagerSetup("Ubuntu.opf", NULL, 0);




	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 },
		//{ "aTangent", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 },
		{ "aBlendIndices", GL_FLOAT, 4 },
		{ "aBlendWeights", GL_FLOAT, 4 }
	};

	vert = (OPshader*)OPcmanLoadGet("Skinning.vert");
	frag = (OPshader*)OPcmanLoadGet("Skinning.frag");
	SKINNING_EFFECT = OPeffectCreate(
		*vert,
		*frag,
		attribs,
		5,
		"Model Effect",
		SKINNING_VERTEX_LAYOUT.stride
	);
}
