#include <RocketEngine/core/Log.h>
#include <RocketEngine/Defines.h>
#include <RKTUtils/Profiling.h>
#include "GameApp.h"

#ifdef RKP_DIST
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
	RKT_LOGGER_INIT();

	RKT_PROFILE_BEGIN_SESSION("Init", "profile_init.json");
	GameApp::initInstance();
	if(!GameApp::getInstance()->initialize())
		return 1;
	RKT_PROFILE_END_SESSION();

	RKT_PROFILE_BEGIN_SESSION("Runtime", "profile_runtime.json");
	GameApp::getInstance()->runGame();
	RKT_PROFILE_END_SESSION();

	RKT_PROFILE_BEGIN_SESSION("Cleanup", "profile_cleanup.json");
	GameApp::cleanInstance();
	RKT_PROFILE_END_SESSION();

	RKT_MEMREPORT();

	return 0;
}