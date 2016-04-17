#include "./GameState.h"
#include "./game/Global.h"
#include "./game/Player.h"
#include "./game/Scene.h"

Scene scene;
Player* player;
OPframeBuffer ShadowFrameBuffer;
OPtexture2D* shadow2D;

#define GROUND 20
OPfloat _quadVertNormData[] = {
	GROUND,  0, GROUND,
	0,  1, 0,
	0,  0,

	-GROUND,  0, GROUND,
	0,  1, 0,
	1,  0,

	-GROUND, 0, -GROUND,
	0,  1, 0,
	1,  1,

	GROUND, 0, -GROUND,
	0,  1, 0,
	0,  1
};

ui16 _quadIndexData[] = {
	2, 1, 0,
	3, 2, 0
};
OPmesh GroundMesh;

void ExampleStateInit(OPgameState* last) {
	GlobalInit();

	//GroundMesh = OPmeshCreate();
	//OPmeshBind(&GroundMesh);
	//OPmeshBuild(
	//	sizeof(OPfloat) * 8, sizeof(ui16),
	//	4, 6,
	//	_quadVertNormData, _quadIndexData
	//);
	//GroundMesh.vertexLayout.stride = sizeof(OPfloat) * 8;


	scene.Init();
	player = scene.AddPlayer(0, "human.opm", "human.png");
	//scene.AddPlayer(1, "human.opm", "human.png");
	scene.AddEnemy("human.opm", "human.png");
	for (ui32 i = 1; i < 2; i++) {
		for (ui32 j = 1; j < 2; j++) {
			f32 offsetX = 485.0f * i;
			f32 offsetZ = 485.0f * j;
			scene.AddStatic("ground.opm", "ground.png")->model.world.Scl(5)->Translate(-485.0f + offsetX, -5, -485.0f + offsetZ);
		}
	}



	//scene.AddCard("Energy.png", EnergyCard);
	//scene.AddCard("Energy.png", EnergyCard);
	//scene.AddCard("Energy.png", EnergyCard);
	//scene.AddCard("Skel.png", SkeletonCard);
	//ground->model.mesh = &GroundMesh;

	//
	//
	//	const GLuint SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	//	GLuint depthMapFBO;
	//	glGenFramebuffers(1, &depthMapFBO);
	//	// - Create depth texture
	//	GLuint depthMap;
	//	glGenTextures(1, &depthMap);
	//	glBindTexture(GL_TEXTURE_2D, depthMap);
	//
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	//	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	//
	//	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	//	glDrawBuffer(GL_NONE);
	//	glReadBuffer(GL_NONE);
	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//
	//
	//
	//	ShadowFrameBuffer.Handle = depthMapFBO;
	//	ShadowFrameBuffer.Texture.Handle = depthMap;
	//	ShadowFrameBuffer.Description.Width = SHADOW_WIDTH;
	//	ShadowFrameBuffer.Description.Height = SHADOW_HEIGHT;
	//	ShadowFrameBuffer.Texture.Description.Width = SHADOW_WIDTH;
	//	ShadowFrameBuffer.Texture.Description.Height = SHADOW_HEIGHT;
	//
	//	OPmaterialAddParam(&MATERIAL, "uShadow", &ShadowFrameBuffer.Texture);
	//
	//
	//
	//	shadow2D = OPtexture2DCreate(&ShadowFrameBuffer.Texture);
	//	shadow2D->Scale = OPvec2Create(0.25, 0.25);
	//	shadow2D->Position = OPvec2Create(0.5, 0.5);
}

i8 started = 0;

OPint ExampleStateUpdate(OPtimer* timer) {

	//SHADOW_CAMERA.pos.x -= 0.01f * timer->Elapsed * OPkeyboardIsDown(OPKEY_J);
	//SHADOW_CAMERA.pos.x += 0.01f * timer->Elapsed * OPkeyboardIsDown(OPKEY_L);
	//SHADOW_CAMERA.pos.y -= 0.01f * timer->Elapsed * OPkeyboardIsDown(OPKEY_U);
	//SHADOW_CAMERA.pos.y += 0.01f * timer->Elapsed * OPkeyboardIsDown(OPKEY_O);
	//SHADOW_CAMERA.pos.z += 0.01f * timer->Elapsed * OPkeyboardIsDown(OPKEY_I);
	//SHADOW_CAMERA.pos.z -= 0.01f * timer->Elapsed * OPkeyboardIsDown(OPKEY_K);
	//SHADOW_CAMERA.Update();

	if (OPgamePadWasReleased(player->controller, OPgamePadButton::OPGAMEPADBUTTON_START) || OPkeyboardWasReleased(OPKEY_ENTER)) {
		started = 1;
	}
	if (!started) return 0;

	if (timer->Elapsed > 200) return 0;

	scene.Update(timer);
	return false;
}

void ExampleStateRender(OPfloat delta) {

	OPrenderCull(1);
	OPrenderDepth(1);
	//{
	//	ShadowFrameBuffer.Bind();
	//	OPrenderClear(0, 0, 0);
	//	//scene.RenderShadows(delta);
	//	ShadowFrameBuffer.Unbind();
	//}

	OPrenderClear(0, 0, 0, 1);
	scene.Render(delta);
	//OPtexture2DRender(shadow2D);
	OPrenderPresent();
}

OPint ExampleStateExit(OPgameState* next) {
	return 0;
}

OPgameState GS_EXAMPLE = {
	ExampleStateInit,
	ExampleStateUpdate,
	ExampleStateRender,
	ExampleStateExit
};
