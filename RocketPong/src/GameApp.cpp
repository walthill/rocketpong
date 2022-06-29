#include "GameApp.h"
#include "RocketEngine.h"
#include "actors/Paddle.h"
#include "actors/Ball.h"
#include "managers/GameManager.h"

GameApp::~GameApp()
{
	clean();
}

void GameApp::clean()
{
	delete mpPlayer1;
	delete mpPlayer2;
	delete mpBall;
	RKTEngine::EngineCore::cleanInstance();
}

bool GameApp::initialize()
{
	RKT_PROFILE_FUNCTION();

	beginInit();

	RKTEngine::EngineCore::initInstance();
	if (!RocketEngine->initialize())
		return false;

	RocketEngine->getMessageManager()->setMessageCallback(RKT_BIND_MESSAGE_FN(GameApp::onMessage));
	RocketEngine->showDebugWireframes(false);

	mpGameManager = new GameManager();

	auto w = RocketEngine->getWindowWidth();
	auto h = RocketEngine->getWindowHeight();

	mpPlayer1 = new Paddle(150);
	mpPlayer1->mpGameObject->name = "P1 Paddle";
	mpPlayer1->mpGameObject->getTransform()->setPosition({ 100, h / 2 });
	mpPlayer1->mpGameObject->getBoxCollider()->setTag("lp");
	mpPlayer2 = new Paddle(150, false);
	mpPlayer2->mpGameObject->name = "P2 Paddle";
	mpPlayer2->mpGameObject->getTransform()->setPosition({ w - 100, h / 2 });
	mpPlayer2->mpGameObject->getBoxCollider()->setTag("rp");

	mpBall = new Ball({ w / 2,h / 2 }, 200);
	mpBall->mpGameObject->name = "Ball";

	generateSceneData(false);
	if (mIsRunning)
	{
		loadStartupScene("game");
	}

	endInit();

	return mIsRunning;
}

void GameApp::beginInit()
{
	pinitPerformanceTracker = new RKTUtil::PerformanceTracker();
	pinitPerformanceTracker->startTracking(mINIT_TRACKER_NAME);
	mIsRunning = true;

#ifdef RKP_DEBUG
	mRunSceneGenerator = true;
#else
	mRunSceneGenerator = false;
#endif // RKP_DEBUG

}

void GameApp::endInit()
{
	pinitPerformanceTracker->stopTracking(mINIT_TRACKER_NAME);
	RKT_INFO("Time to init: " + std::to_string(pinitPerformanceTracker->getElapsedTime(mINIT_TRACKER_NAME)) + "ms\n");

	delete pinitPerformanceTracker;

	mpPerformanceTracker = new RKTUtil::PerformanceTracker();
	mpFrameTimer = new RKTUtil::Timer();
}

void GameApp::loadStartupScene(const std::string& scene)
{
	RocketEngine->getSceneManager()->loadScene(scene);
}

void GameApp::generateSceneData(bool quitOnComplete)
{
	if (mRunSceneGenerator)
	{
		auto w = RocketEngine->getWindowWidth();
		auto h = RocketEngine->getWindowHeight();

		RocketEngine->getSceneManager()->beginScene("game");
		UILabel* p1Score = new UILabel({ w / 2 - 32, 10 }, std::to_string(mpGameManager->mP1Score));
		p1Score->mpGameObject->name = "p1score";
		UILabel* p2Score = new UILabel({ w / 2 + 32, 10 }, std::to_string(mpGameManager->mP2Score));
		p2Score->mpGameObject->name = "p2score";
		RocketEngine->getSceneManager()->endScene();

		RocketEngine->getSceneManager()->beginScene("main");
		UILabel* main1 = new UILabel({ w / 2 - 32, 10 }, "MAIN");
		UILabel* main2 = new UILabel({ w / 2 + 32, 10 }, "MAIN");
		RocketEngine->getSceneManager()->endScene();
	}

	if (quitOnComplete)
		quit(RKTEngine::ExitMessage());	
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

#ifdef RKP_DEBUG  
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
	RocketEngine->update();

#ifdef RKP_DEBUG  
	if (Input::getKeyDown(KeyCode::Escape))
	{
		mIsRunning = false;
	}
	if (Input::getKeyDown(KeyCode::R))
	{
		mpBall->reset();
		mpGameManager->reset();
	}
	if (Input::getKeyDown(KeyCode::Q))
	{
		if (RocketEngine->getSceneManager()->isActiveScene("game"))
			RocketEngine->getSceneManager()->loadScene("main");
		else
			RocketEngine->getSceneManager()->loadScene("game");
	}
#endif
}

void GameApp::render()
{
	RocketEngine->render();
}

void GameApp::onMessage(RKTEngine::Message& message)
{
	RKTEngine::MessageDispatcher dispatcher(message);
	dispatcher.dispatch<RKTEngine::ExitMessage>(RKT_BIND_MESSAGE_FN(GameApp::quit));

/*#ifdef RKP_DEBUG  
	dispatcher.dispatch<RKTEngine::DebugMessage>(RKT_BIND_MESSAGE_FN(GameApp::debugControls));
#endif*/

	RocketEngine->onMessage(message);
}


GameManager* GameApp::getGameManager()
{
	return mpGameManager;
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