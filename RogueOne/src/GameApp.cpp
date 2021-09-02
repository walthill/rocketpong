#include "GameApp.h"
#include "RocketEngine.h"
#include "actors/player/Player.h"
#include "map/Map.h"
#include "map/DungeonGenerator.h"

Player* GameApp::spPlayer = nullptr;

GameApp::~GameApp()
{
	clean();
}

void GameApp::clean()
{
	int amt = RKTEngine::EngineCore::getInstance()->getEntityManager()->getNumGameObjects();
	RKT_TRACE("GameObjs: " + std::to_string(amt));
	if (dungeonGen != nullptr)
		delete dungeonGen;

	//delete mpMap;
	for (auto map : maps)
	{
		delete map;
	}
	maps.clear();
	
	delete spPlayer;

	RKTEngine::EngineCore::cleanInstance();	
}

bool GameApp::initialize()
{
	beginInit();

	RKTEngine::EngineCore::initInstance();
	if (!RKTEngine::EngineCore::getInstance()->initialize())
		return false;

	RKTEngine::EngineCore::getInstance()->getMessageManager()->setMessageCallback(RKT_BIND_MESSAGE_FN(GameApp::onMessage));

	//auto pEntityManager = RKTEngine::EngineCore::getInstance()->getEntityManager();

	//mpMap = new Map(10, 10, 16);

	maps = std::vector<Map*>();
	dungeonGen = new DungeonGenerator(1280 * .75f, 720);
	const auto& rooms = dungeonGen->getRoomDisplayData();
	for (const auto& room : rooms)
	{
		maps.push_back(new Map(room));
	}
	
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
		generateDungeon();
		//mIsRunning = false;
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

void GameApp::generateDungeon()
{
	if (dungeonGen != nullptr)
		delete dungeonGen;

	for (auto map : maps)
	{
		delete map;
	}
	maps.clear();

	maps = std::vector<Map*>();
	dungeonGen = new DungeonGenerator(1280 * .75f, 720);
	auto& rooms = dungeonGen->getRoomDisplayData();
	for (const auto& room : rooms)
	{
		maps.push_back(new Map(room));
	}

	int amt = RKTEngine::EngineCore::getInstance()->getEntityManager()->getNumGameObjects();
	RKT_TRACE("GameObjs: " + std::to_string(amt));
}

double GameApp::getTime()
{
	return RKTEngine::EngineCore::getInstance()->getTime();
}

Map* GameApp::getMap()
{
	return mpMap;
}

bool GameApp::quit(RKTEngine::ExitMessage& msg)
{
	mIsRunning = false;
	return true;
}