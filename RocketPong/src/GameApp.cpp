#include "GameApp.h"
#include "RocketEngine.h"
#include "actors/Paddle.h"
#include "actors/Ball.h"

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
	if (!RKTEngine::EngineCore::getInstance()->initialize())
		return false;

	RocketEngine->getMessageManager()->setMessageCallback(RKT_BIND_MESSAGE_FN(GameApp::onMessage));
	RocketEngine->showDebugWireframes(false);

	auto w = RocketEngine->getWindowWidth();
	auto h = RocketEngine->getWindowHeight();

	//mpPlayer1 = new Paddle(150);
	//mpPlayer1->mpGameObject->getTransform()->setPosition({ 100, h / 2 });
	//mpPlayer2 = new Paddle(150, false);
	//mpPlayer2->mpGameObject->getTransform()->setPosition({ w - 100, h / 2 });

	mpBall = new Ball({ w / 2,h / 2 }, 200);

	UILabel* label = new UILabel({100,100}, "FOO");

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

	RKTEngine::EngineCore::getInstance()->onMessage(message);
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