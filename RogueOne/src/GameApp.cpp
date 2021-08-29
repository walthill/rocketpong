#include "GameApp.h"
#include "RocketEngine.h"
#include "actors/player/Player.h"
#include "map/Map.h"

Player* GameApp::spPlayer = nullptr;

GameApp::~GameApp()
{
	clean();
}

void GameApp::clean()
{
	RKTEngine::EngineCore::cleanInstance();
	delete mpMap;
	delete spPlayer;
}

bool GameApp::initialize()
{
	beginInit();

	RKTEngine::EngineCore::initInstance();
	if (!RKTEngine::EngineCore::getInstance()->initialize())
		return false;

	RKTEngine::EngineCore::getInstance()->getMessageManager()->setMessageCallback(RKT_BIND_MESSAGE_FN(GameApp::onMessage));

	//auto pEntityManager = RKTEngine::EngineCore::getInstance()->getEntityManager();

	mpMap = new Map(10, 10, 16);

	spPlayer = new Player("tileset_0", "player", 16, 16, glm::vec2(16, 16));
	
	endInit();

	return mIsRunning;
}

void GameApp::beginInit()
{
	pinitPerformanceTracker = new RKTUtil::PerformanceTracker();
	pinitPerformanceTracker->startTracking(mINIT_TRACKER_NAME);
}

void GameApp::endInit()
{
	pinitPerformanceTracker->stopTracking(mINIT_TRACKER_NAME);
	RKT_INFO("Time to init: " + std::to_string(pinitPerformanceTracker->getElapsedTime(mINIT_TRACKER_NAME)) + "ms\n");

	delete pinitPerformanceTracker;

	mpPerformanceTracker = new RKTUtil::PerformanceTracker();
	mpFrameTimer = new RKTUtil::Timer();

	mIsRunning = true;
}

bool GameApp::runGame()
{
	while (mIsRunning)
	{
		mpPerformanceTracker->startTracking(mLOOP_TRACKER_NAME);
		mpFrameTimer->start();
		mpPerformanceTracker->startTracking(mDRAW_TRACKER_NAME);

		update();
		render();

		mpPerformanceTracker->stopTracking(mDRAW_TRACKER_NAME);
		mpFrameTimer->sleepUntilElapsed(RKTEngine::FRAME_TIME_60FPS);
		mpPerformanceTracker->stopTracking(mLOOP_TRACKER_NAME);

#ifdef RG_DEBUG  
		if (mDisplayFrameTime)
		{
			if (!mShowFPS)
			{
				RKT_TRACE("loop took:" + std::to_string(mpPerformanceTracker->getElapsedTime(mLOOP_TRACKER_NAME)) +
					"ms draw took:" + std::to_string(mpPerformanceTracker->getElapsedTime(mDRAW_TRACKER_NAME)) + "ms\n");
			}
			else
			{
				mFPS = (int)(1000.0 / mpPerformanceTracker->getElapsedTime(mDRAW_TRACKER_NAME));
				RKT_TRACE("FPS: " + std::to_string(mFPS));
			}
		}
#endif  
	}

	delete mpPerformanceTracker;
	delete mpFrameTimer;

	mIsRunning = false;
	return mIsRunning;
}

void GameApp::update()
{
	RKTEngine::EngineCore::getInstance()->update();

	if (Input::getKeyDown(KeyCode::Escape))
	{
		mIsRunning = false;
	}
}

void GameApp::render()
{
	RKTEngine::EngineCore::getInstance()->render();
}

void GameApp::onMessage(RKTEngine::Message& message)
{
	RKTEngine::MessageDispatcher dispatcher(message);
	dispatcher.dispatch<RKTEngine::ExitMessage>(RKT_BIND_MESSAGE_FN(GameApp::quit));

	RKTEngine::EngineCore::getInstance()->onMessage(message);
	spPlayer->onMessage(message);
}

double GameApp::getTime()
{
	return RKTEngine::EngineCore::getInstance()->getTime();
}

bool GameApp::quit(RKTEngine::ExitMessage& msg)
{
	mIsRunning = false;
	return true;
}