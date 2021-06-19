#include "core/RK_Log.h"

int main()
{
	RKT_LOGGER_INIT();
	RKT_CORE_INFO("Rocket Engine Core Logger initialized");

	/*	
	GameApp::initInstance();

	if (!GameApp::getInstance()->run())
		GameApp::cleanInstance();
	*/

	RKT_MEMREPORT();

	return 0;
}