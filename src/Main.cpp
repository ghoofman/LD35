//////////////////////////////////////
// Application Entry Point
//////////////////////////////////////
#include "./OPengine.h"
#include "Main.h"
#include "GameState.h"

//////////////////////////////////////
// Application Methods
//////////////////////////////////////

void ApplicationInit() {
	OP_LOG_LEVEL = 2000;
	OPloadersAddDefault();
	OPskeletonAddLoader();
	OPskeletonAnimationAddLoader();
	OPcmanInit(OPIFEX_ASSETS);
	OPrenderInit();

	OPfmodInit();
	OPphysXInit();

	OPgamePadSetDeadZones(0.3);

	OPgameStateChange(&GS_EXAMPLE);
}

OPint ApplicationUpdate(OPtimer* timer) {
	OPfmodUpdate();

	OPinputSystemUpdate(timer);

#ifdef _DEBUG
	if (OPkeyboardWasReleased(OPKEY_ESCAPE)) {
		return 1;
	}
#endif

	return ActiveState->Update(timer);
}

void ApplicationRender(OPfloat delta) {
	ActiveState->Render(delta);
}

void ApplicationDestroy() {
}

void ApplicationSetup() {
	OPinitialize = ApplicationInit;
	OPupdate = ApplicationUpdate;
	OPrender = ApplicationRender;
	OPdestroy = ApplicationDestroy;
}

//////////////////////////////////////
// Application Entry Point
//////////////////////////////////////
OP_MAIN {
	OPlog("Starting up OPifex Engine");

	ApplicationSetup();

	OP_MAIN_START
	OP_MAIN_END
	OP_MAIN_SUCCESS
}
