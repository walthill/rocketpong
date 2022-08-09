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
	RKT_PROFILE_FUNCTION();

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
	RKT_PROFILE_FUNCTION();

	pinitPerformanceTracker->stopTracking(mINIT_TRACKER_NAME);
	RKT_INFO("Time to init: " + std::to_string(pinitPerformanceTracker->getElapsedTime(mINIT_TRACKER_NAME)) + "ms\n");

	delete pinitPerformanceTracker;

	mpPerformanceTracker = new RKTUtil::PerformanceTracker();
	mpFrameTimer = new RKTUtil::Timer();
}

void GameApp::loadStartupScene(const std::string& scene)
{
	RKT_PROFILE_FUNCTION();
	RocketEngine->getSceneManager()->loadScene(scene);
}

void GameApp::generateSceneData(bool quitOnComplete)
{
	RKT_PROFILE_FUNCTION();

	if (mRunSceneGenerator)
	{
		auto w = RocketEngine->getWindowWidth();
		auto h = RocketEngine->getWindowHeight();

		RocketEngine->getSceneManager()->beginScene("game");

		GameObject* pPlayer1 = GameObjManager->createActor();
		pPlayer1->getScript()->bind<Paddle>(pPlayer1->getId());
		pPlayer1->getScript()->get<Paddle>()->mIsP1 = true;
		pPlayer1->name = "P1 Paddle";
		pPlayer1->getTransform()->setPosition({ 100, h / 2 });
		pPlayer1->getBoxCollider()->setTag("lp");
		
		GameObject* mpPlayer2 = GameObjManager->createActor();
		mpPlayer2->getScript()->bind<Paddle>(mpPlayer2->getId());
		mpPlayer2->getScript()->get<Paddle>()->mIsP1 = false;
		mpPlayer2->name = "P2 Paddle";
		mpPlayer2->getTransform()->setPosition({ w - 100, h / 2 });
		mpPlayer2->getBoxCollider()->setTag("rp");
		
		mpBall = GameObjManager->createActor();
		mpBall->getScript()->bind<Ball>(mpBall->getId());
		mpBall->getScript()->get<Ball>()->mStartPos = { w / 2,h / 2 };
		mpBall->name = "Ball";

		//midline
		auto midlineSprite = RocketEngine->getEntityManager()->createSprite("paddle", {w / 2, h / 2}, {0.25f, 100.0f});
		midlineSprite->getSprite()->setColor(RKTEngine::Color(127.5f, 127.5f, 127.5f, .2f));
		midlineSprite->name = "midline";

		GameObject* p1Score = GameObjManager->createLabel(std::to_string(mpGameManager->mP1Score), { w / 2 - 72, 10 }, glm::vec2(1.0f), 0, "Arkitech-Medium", RKTEngine::Text::sDefaultTextSize);
		p1Score->name = "p1score";
		GameObject* p2Score = GameObjManager->createLabel(std::to_string(mpGameManager->mP2Score), { w / 2 + 32, 10 }, glm::vec2(1.0f), 0, "Arkitech-Medium", RKTEngine::Text::sDefaultTextSize);
		p2Score->name = "p2score";
		mpGameManager->setScoreUI(p1Score->getId(), p2Score->getId());
		
		RocketEngine->getSceneManager()->endScene(false);

		RocketEngine->getSceneManager()->beginScene("main");
		GameObject* main1 = GameObjManager->createLabel("MAIN", { w / 2 - 32, 10 });
		GameObject* main2 = GameObjManager->createLabel("MAIN", { w / 2 + 32, 10 });
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
		mpBall->getScript()->get<Ball>()->reset();
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