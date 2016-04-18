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


#include "game\CardDesc.h"
#include "game\cards\wolf.h"
#include "game\cards\energy.h"
#include "game\cards\skeleton.h"
#include "game\cards\devilshowl.h"

Card cards[4];
i8* counts[4];
i32 cardInd;
OPfmodSound* bg;
OPfmodChannel* bgChannel;

OPspriteSystem spriteSystem;
OPeffect spriteEffect;
OPsprite* sprites[30];
OPcam camera;

OPspriteSystemSprite* s_A;
OPspriteSystemSprite* s_S;
OPspriteSystemSprite* s_D;
OPspriteSystemSprite* s_W;
OPspriteSystemSprite* s_Q;
OPspriteSystemSprite* s_E;
OPspriteSystemSprite* s_R;
OPspriteSystemSprite* s_Enter;
OPspriteSystemSprite* s_Mouse;
OPspriteSystemSprite* s_Controller;

const OPchar* message;

void ExampleStateInit(OPgameState* last) {
	GlobalInit();


	OPcmanLoad("mousekb.opss");
	OPspriteSystemEffectDefault(&spriteEffect);
	sprites[0] = (OPsprite*)OPcmanGet("mousekb/A");
	sprites[1] = (OPsprite*)OPcmanGet("mousekb/Aon");
	sprites[2] = (OPsprite*)OPcmanGet("mousekb/S");
	sprites[3] = (OPsprite*)OPcmanGet("mousekb/Son");
	sprites[4] = (OPsprite*)OPcmanGet("mousekb/D");
	sprites[5] = (OPsprite*)OPcmanGet("mousekb/Don");
	sprites[6] = (OPsprite*)OPcmanGet("mousekb/W");
	sprites[7] = (OPsprite*)OPcmanGet("mousekb/Won");
	sprites[8] = (OPsprite*)OPcmanGet("mousekb/Q");
	sprites[9] = (OPsprite*)OPcmanGet("mousekb/Qon");
	sprites[10] = (OPsprite*)OPcmanGet("mousekb/E");
	sprites[11] = (OPsprite*)OPcmanGet("mousekb/Eon");
	sprites[12] = (OPsprite*)OPcmanGet("mousekb/R");
	sprites[13] = (OPsprite*)OPcmanGet("mousekb/Ron");
	sprites[14] = (OPsprite*)OPcmanGet("mousekb/Enter");
	sprites[15] = (OPsprite*)OPcmanGet("mousekb/Enteron");
	sprites[16] = (OPsprite*)OPcmanGet("mousekb/mouse");
	sprites[17] = (OPsprite*)OPcmanGet("mousekb/mouseLeft");
	sprites[18] = (OPsprite*)OPcmanGet("mousekb/mouseRight");
	sprites[19] = (OPsprite*)OPcmanGet("mousekb/mouseWheel");
	sprites[20] = (OPsprite*)OPcmanGet("mousekb/controller");
	OPspriteSystemInit(&spriteSystem, sprites, 12, &spriteEffect, OPSPRITESYSTEMALIGN_BOTTOM_LEFT);

	s_A = OPspriteSystemAdd(&spriteSystem); s_A->CurrentSprite = 0; s_A->Position = OPvec2Create(50, OPRENDER_HEIGHT - 120);
	s_S = OPspriteSystemAdd(&spriteSystem); s_S->CurrentSprite = 2; s_S->Position = OPvec2Create(50, OPRENDER_HEIGHT - 120);
	s_D = OPspriteSystemAdd(&spriteSystem); s_D->CurrentSprite = 4; s_D->Position = OPvec2Create(50, OPRENDER_HEIGHT - 120);
	s_W = OPspriteSystemAdd(&spriteSystem); s_W->CurrentSprite = 6; s_W->Position = OPvec2Create(50, OPRENDER_HEIGHT - 120);
	s_Q = OPspriteSystemAdd(&spriteSystem); s_Q->CurrentSprite = 8; s_Q->Position = OPvec2Create(50, OPRENDER_HEIGHT - 120);
	s_E = OPspriteSystemAdd(&spriteSystem); s_E->CurrentSprite = 10; s_E->Position = OPvec2Create(50, OPRENDER_HEIGHT - 120);
	s_R = OPspriteSystemAdd(&spriteSystem); s_R->CurrentSprite = 12; s_R->Position = OPvec2Create(50, OPRENDER_HEIGHT - 120);
	s_Enter = OPspriteSystemAdd(&spriteSystem); s_Enter->CurrentSprite = 14; s_Enter->Position = OPvec2Create(50, OPRENDER_HEIGHT - 120);
	s_Mouse = OPspriteSystemAdd(&spriteSystem); s_Mouse->CurrentSprite = 16; s_Mouse->Position = OPvec2Create(250, OPRENDER_HEIGHT - 120);
	s_Controller = OPspriteSystemAdd(&spriteSystem); s_Controller->CurrentSprite = 20; s_Controller->Position = OPvec2Create(320, OPRENDER_HEIGHT - 112);

	camera = OPcamOrtho(OPvec3Create(0, 0, 10), OPVEC3_ZERO, OPVEC3_UP, 0.1f, 20.0f, 0, (OPfloat)OPRENDER_WIDTH, 0, (OPfloat)OPRENDER_HEIGHT);


	bg = OPfmodLoad("Audio/BlueOceans.mp3");
	bgChannel = OPfmodPlay(bg);
	//GroundMesh = OPmeshCreate();
	//OPmeshBind(&GroundMesh);
	//OPmeshBuild(
	//	sizeof(OPfloat) * 8, sizeof(ui16),
	//	4, 6,
	//	_quadVertNormData, _quadIndexData
	//);
	//GroundMesh.vertexLayout.stride = sizeof(OPfloat) * 8;

	cards[0].Init(CardEnergy());
	cards[1].Init(CardSkeleton());
	cards[2].Init(CardWolf());
	cards[3].Init(CardDevilsHowl());
	cardInd = 0;
	counts[0] = "x20";
	counts[1] = "x2";
	counts[2] = "x2";
	counts[3] = "x4";

	scene.Init();
	player = scene.AddPlayer(0, "ld35person.opm", "human.png");
	//scene.AddPlayer(1, "ld35person.opm", "human.png");
	scene.AddEnemy("ld35person.opm", "human.png");
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
	if (!OPfmodIsPlaying(bgChannel)) {
		bgChannel = OPfmodPlay(bg);
	}

	if (OPgamePadWasReleased(player->controller, OPgamePadButton::OPGAMEPADBUTTON_START) || OPkeyboardWasReleased(OPKEY_ENTER)) {
		started = 1;
	}
	if (!started) {
		// Update the menu
		message = "";

		if (OPkeyboardWasPressed(OPKEY_A)) { s_A->CurrentSprite++; }
		if (OPkeyboardWasReleased(OPKEY_A)) { s_A->CurrentSprite--; }
		if (OPkeyboardIsDown(OPKEY_A) || OPgamePadLeftThumbIsLeft(player->controller)) { message = "Move left"; }

		if (OPkeyboardWasPressed(OPKEY_S)) { s_S->CurrentSprite++; }
		if (OPkeyboardWasReleased(OPKEY_S)) { s_S->CurrentSprite--; }
		if (OPkeyboardIsDown(OPKEY_S) || OPgamePadLeftThumbIsDown(player->controller)) { message = "Move down"; }

		if (OPkeyboardWasPressed(OPKEY_D)) { s_D->CurrentSprite++; }
		if (OPkeyboardWasReleased(OPKEY_D)) { s_D->CurrentSprite--; }
		if (OPkeyboardIsDown(OPKEY_D) || OPgamePadLeftThumbIsRight(player->controller)) { message = "Move right"; }

		if (OPkeyboardWasPressed(OPKEY_W)) { s_W->CurrentSprite++; }
		if (OPkeyboardWasReleased(OPKEY_W)) { s_W->CurrentSprite--; }
		if (OPkeyboardIsDown(OPKEY_W) || OPgamePadLeftThumbIsUp(player->controller)) { message = "Move forward"; }

		if (OPkeyboardWasPressed(OPKEY_Q)) { s_Q->CurrentSprite++; }
		if (OPkeyboardWasReleased(OPKEY_Q)) { s_Q->CurrentSprite--; }
		if (OPkeyboardIsDown(OPKEY_Q) || OPgamePadRightThumbIsLeft(player->controller) || OPgamePadIsDown(player->controller, OPgamePadButton::OPGAMEPADBUTTON_X)) { message = "Prev card in hand"; }

		if (OPkeyboardWasPressed(OPKEY_E)) { s_E->CurrentSprite++; }
		if (OPkeyboardWasReleased(OPKEY_E)) { s_E->CurrentSprite--; }
		if (OPkeyboardIsDown(OPKEY_E) || OPgamePadRightThumbIsRight(player->controller) || OPgamePadIsDown(player->controller, OPgamePadButton::OPGAMEPADBUTTON_B)) { message = "Next card in hand"; }

		if (OPkeyboardWasPressed(OPKEY_R)) { s_R->CurrentSprite++; }
		if (OPkeyboardWasReleased(OPKEY_R)) { s_R->CurrentSprite--; }
		if (OPkeyboardIsDown(OPKEY_R) || OPgamePadIsDown(player->controller, OPgamePadButton::OPGAMEPADBUTTON_Y)) { message = "Discard current card in hand"; }

		if (OPkeyboardWasPressed(OPKEY_ENTER)) { s_Enter->CurrentSprite++; }
		if (OPkeyboardWasReleased(OPKEY_ENTER)) { s_Enter->CurrentSprite--; }
		if (OPkeyboardIsDown(OPKEY_ENTER) || OPgamePadIsDown(player->controller, OPgamePadButton::OPGAMEPADBUTTON_START)) { message = "Start"; }

		if (OPmouseWasPressed(OPMOUSE_LBUTTON)) { s_Mouse->CurrentSprite = 17; }
		if (OPmouseWasReleased(OPMOUSE_LBUTTON)) { s_Mouse->CurrentSprite = 16; }
		if (OPmouseIsDown(OPMOUSE_LBUTTON) || OPgamePadRightTriggerIsDown(player->controller)) { message = "Attack"; }

		if (OPmouseWasPressed(OPMOUSE_RBUTTON)) { s_Mouse->CurrentSprite = 18; }
		if (OPmouseWasReleased(OPMOUSE_RBUTTON)) { s_Mouse->CurrentSprite = 16; }
		if (OPmouseIsDown(OPMOUSE_RBUTTON) || OPgamePadIsDown(player->controller, OPgamePadButton::OPGAMEPADBUTTON_A)) { message = "Use card"; }

		if(OPmouseWheelMoved() != 0) { s_Mouse->CurrentSprite = 19; message = "Next/Prev card"; }
		//OPspriteSystemSprite* s_A;
		//OPspriteSystemSprite* s_S;
		//OPspriteSystemSprite* s_D;
		//OPspriteSystemSprite* s_W;
		//OPspriteSystemSprite* s_Q;
		//OPspriteSystemSprite* s_E;
		//OPspriteSystemSprite* s_R;
		//OPspriteSystemSprite* s_Enter;
		//OPspriteSystemSprite* s_Mouse;

		if (OPgamePadRightThumbNowLeft(player->controller) || OPgamePadWasReleased(player->controller, OPgamePadButton::OPGAMEPADBUTTON_X) || OPkeyboardWasReleased(OPKEY_Q) || OPmouseWheelMoved() < 0) {
			cardInd--;
			if (cardInd < 0) cardInd = 3;
		}

		if (OPgamePadRightThumbNowRight(player->controller) || OPgamePadWasReleased(player->controller, OPgamePadButton::OPGAMEPADBUTTON_B) || OPkeyboardWasReleased(OPKEY_E) || OPmouseWheelMoved() > 0) {
			cardInd++;
			cardInd = cardInd % 4;
		}

		return 0;
	}

	if (timer->Elapsed > 200) return 0;

	if (scene.winner == 0) {
		scene.Update(timer);
	}

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

	if (!started) {
		// Render the menu
		OPfontRenderBegin(FONTMANAGER);
		OPfontManagerSetAlign(OPfontAlign::OPFONT_ALIGN_CENTER);
		FONTMANAGER->scale = 1.0;
		OPfontManagerSetColor(OPvec3Create(1, 1, 1));
		OPfontRender("Your Deck", OPvec2Create(OPRENDER_WIDTH / 2.0, 50));

		OPfontManagerSetAlign(OPfontAlign::OPFONT_ALIGN_LEFT);

		FONTMANAGER->scale = 0.5;
		OPfontRender("Press Back to quit", OPvec2Create(50, OPRENDER_HEIGHT - 50));


		OPfontRender("Controls", OPvec2Create(50, 20));
		OPfontRender(message, OPvec2Create(50, 125));

		OPfontManagerSetAlign(OPfontAlign::OPFONT_ALIGN_LEFT);
		OPfontRender("Press Start to begin", OPvec2Create(OPRENDER_WIDTH - 260, OPRENDER_HEIGHT - 50));

		f32 cardDist = (OPRENDER_WIDTH * 0.3 * 0.5);
		f32 offset = -cardInd * (OPRENDER_WIDTH * 0.3 * 0.5);
		for (ui32 i = 0; i < 4; i++) {
			OPfontManagerSetAlign(OPfontAlign::OPFONT_ALIGN_CENTER);
			FONTMANAGER->scale = 1.0;
			OPfontManagerSetColor(OPvec3Create(1, 1, 1));
			OPfontRender(counts[i], OPvec2Create(OPRENDER_WIDTH / 2.0 + offset + i * cardDist, 550 - (cardInd != i) * 50));
		}

		OPfontRenderEnd();

		offset = -cardInd * 0.3;
		for (ui32 i = 0; i < 4; i++) {
			cards[i].RenderForMenu(offset + i * 0.3, i == cardInd);
		}

		OPrenderDepth(0);
		OPtexturePixelate();
		OPspriteSystemRender(&spriteSystem, &camera);
	}
	else {
		if (scene.winner == 0) {
			scene.Render(delta);
		}
		else {
			OPfontRenderBegin(FONTMANAGER);
			OPfontManagerSetAlign(OPfontAlign::OPFONT_ALIGN_CENTER);
			FONTMANAGER->scale = 1.0;
			if (scene.winner == 1) {
				OPfontManagerSetColor(OPvec3Create(0, 0, 1));
				OPfontRender("You won the fight", OPvec2Create(OPRENDER_WIDTH / 2.0, 50));
			} else {
				OPfontManagerSetColor(OPvec3Create(1, 0, 0));
				OPfontRender("You lost the fight", OPvec2Create(OPRENDER_WIDTH / 2.0, 50));
			}
			OPfontRenderEnd();
		}
	}
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
