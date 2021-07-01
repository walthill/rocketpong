#include <RocketEngine/core/Log.h>
#include "GameApp.h"

int main()
{
	RKT_LOGGER_INIT();
	RKT_CORE_INFO("Rocket Engine Core Logger initialized");

	GameApp::initInstance();

	GameApp::getInstance()->initialize();

	if (!GameApp::getInstance()->runGame())
	{
		GameApp::cleanInstance();
	}

	RKT_MEMREPORT();

	return 0;
}