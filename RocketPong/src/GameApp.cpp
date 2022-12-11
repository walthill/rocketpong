#include "GameApp.h"
#include "RocketEngine.h"
#include "actors/Paddle.h"
#include "actors/Ball.h"
#include "managers/GameManager.h"
#include "managers/UIManager.h"

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

	generateSceneData(false);
	if (mIsRunning)
	{
		loadStartupScene("main");
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
		{
			GameObject* gameMan = GameObjManager->createActor();
			gameMan->name = "GameManager";
			gameMan->getNativeScript()->bind<GameManager>(gameMan->getId());
			auto pGameManager = gameMan->getNativeScript()->get<GameManager>();
			
			GameObject* pPlayer1 = GameObjManager->createActor();
			pPlayer1->getNativeScript()->bind<Paddle>(pPlayer1->getId());
			pPlayer1->getNativeScript()->get<Paddle>()->mIsP1 = true;
			pPlayer1->name = "P1 Paddle";
			pPlayer1->getTransform()->setPosition({ 100, h / 2 });
			GameObjManager->addSprite(pPlayer1->getId(), "paddle");
			auto spr = pPlayer1->getSprite();
			GameObjManager->addBoxCollider(pPlayer1->getId(), spr->getData()->mWidth, spr->getData()->mHeight);
			pPlayer1->getBoxCollider()->setTag("lp");

			GameObject* pPlayer2 = GameObjManager->createActor();
			pPlayer2->getNativeScript()->bind<Paddle>(pPlayer2->getId());
			pPlayer2->getNativeScript()->get<Paddle>()->mIsP1 = false;
			pPlayer2->name = "P2 Paddle";
			pPlayer2->getTransform()->setPosition({ w - 100, h / 2 });
			GameObjManager->addSprite(pPlayer2->getId(), "paddle");
			spr = pPlayer2->getSprite();
			GameObjManager->addBoxCollider(pPlayer2->getId(), spr->getData()->mWidth, spr->getData()->mHeight);
			pPlayer2->getBoxCollider()->setTag("rp");

			//midline
			auto midlineSprite = RocketEngine->getEntityManager()->createSprite("paddle", { w / 2, h / 2 }, { 0.25f, 100.0f });
			midlineSprite->getSprite()->setColor(RKTEngine::Color(127.5f, 127.5f, 127.5f, 255 * .35f));
			midlineSprite->name = "midline";

			auto pBall = GameObjManager->createActor();
			pBall->getNativeScript()->bind<Ball>(pBall->getId());
			pBall->getNativeScript()->get<Ball>()->mStartPos = { w / 2,h / 2 };
			pBall->name = "Ball";
			GameObjManager->addSprite(pBall->getId(), "ball");
			spr = pBall->getSprite();
			GameObjManager->addBoxCollider(pBall->getId(), spr->getData()->mWidth / 2, spr->getData()->mHeight / 2);
			GameObjManager->addAudioSource(pBall->getId(), "winwin");

			GameObject* p1Score = GameObjManager->createLabel(std::to_string(pGameManager->mP1Score), { w / 2 - 72, 10 }, glm::vec2(1.0f), 0, "Arkitech-Medium");
			p1Score->name = "p1score";
			GameObject* p2Score = GameObjManager->createLabel(std::to_string(pGameManager->mP2Score), { w / 2 + 32, 10 }, glm::vec2(1.0f), 0, "Arkitech-Medium");
			p2Score->name = "p2score";
			GameObject* pause = GameObjManager->createLabel("Paused", { w / 2 - 110, h / 6 }, glm::vec2(1.0f), 0, "Arkitech-Medium", 34);
			pause->name = "pause-text";
			pause->getUILabel()->setEnabled(false);

			GameObject* pause1 = GameObjManager->createButton();
			pause1->getButton()->setFont("Arkitech-Medium");
			pause1->getButton()->setText("Play");
			pause1->getButton()->setFontSize(22);
			pause1->getTransform()->setPosition({ w / 2, h / 2 - 64 });
			pause1->getButton()->setEnabled(false);
			GameObject* pause2 = GameObjManager->createButton();
			pause2->getButton()->setFont("Arkitech-Medium");
			pause2->getButton()->setText("Quit");
			pause2->getButton()->setFontSize(22);
			pause2->getTransform()->setPosition({ w / 2, h / 2 });
			pause2->getButton()->setEnabled(false);

			pGameManager->resumeButtonId = pause1->getId();
			pGameManager->quitButtonId = pause2->getId();
			pGameManager->pauseTextId = pause->getId();
			pGameManager->setBallHandle(pBall->getId());
			pGameManager->setScoreUI(p1Score->getId(), p2Score->getId());
		}
		RocketEngine->getSceneManager()->endScene();

		RocketEngine->getSceneManager()->beginScene("main");
		{
			GameObject* gameMan = GameObjManager->createActor();
			gameMan->name = "UIManager";
			gameMan->getNativeScript()->bind<UIManager>(gameMan->getId());
			auto pUIManager = gameMan->getNativeScript()->get<UIManager>();

			GameObject* title = GameObjManager->createLabel("Pong Type-R", { w / 2 - 200, h / 5}, glm::vec2(1.0f), 0, "Arkitech-Medium", 34);
			title->name = "title-text";
			GameObject* credits = GameObjManager->createLabel("a walter hill game", { 10, h - 32 }, glm::vec2(1.0f), 0, "Calibri", 18);
			credits->name = "credits-text";

			GameObject* main1 = GameObjManager->createButton();
			main1->getButton()->setFontSize(22);
			main1->getButton()->setFont("Arkitech-Medium");
			main1->getButton()->setText("Play");
			main1->getTransform()->setPosition({ w / 2, h / 2 - 64});
			GameObject* main2 = GameObjManager->createButton();
			main2->getButton()->setFontSize(22);
			main2->getButton()->setFont("Arkitech-Medium");
			main2->getButton()->setText("Help");
			main2->getTransform()->setPosition({ w / 2, h / 2});
			GameObject* main3 = GameObjManager->createButton();
			main3->getButton()->setFontSize(22);
			main3->getButton()->setFont("Arkitech-Medium");
			main3->getButton()->setText("Quit");
			main3->getTransform()->setPosition({ w / 2, h / 2 + 64});

			pUIManager->playButtonId = main1->getId();
			pUIManager->controlsButtonId = main2->getId();
			pUIManager->exitButtonId = main3->getId();
		}
		RocketEngine->getSceneManager()->endScene();

		RocketEngine->getSceneManager()->beginScene("controls");
		{
			GameObject* gameMan = GameObjManager->createActor();
			gameMan->name = "UIManager";
			gameMan->getNativeScript()->bind<UIManager>(gameMan->getId());
			auto pUIManager = gameMan->getNativeScript()->get<UIManager>();

			GameObject* title = GameObjManager->createLabel("Help", { w / 2 - 64, h / 6 }, glm::vec2(1.0f), 0, "Arkitech-Medium", 34);
			title->name = "title-text";

			GameObject* p1Text = GameObjManager->createLabel("Move: W/S + P/L Keys", {w / 3.5f, h / 2 - 128}, glm::vec2(1.0f), 0, "Arkitech-Medium", 18);
			p1Text->name = "p1-text";
			GameObject* p2Text = GameObjManager->createLabel("Fast Move: Shift Key", { w / 3.5f, h / 2 - 64 }, glm::vec2(1.0f), 0, "Arkitech-Medium", 18);
			p2Text->name = "p2-text";
			GameObject* pause = GameObjManager->createLabel("Pause Game: Escape", { w / 3.5f, h / 2 }, glm::vec2(1.0f), 0, "Arkitech-Medium", 18);
			pause->name = "pause-text";

			GameObject* backButton = GameObjManager->createButton();
			backButton->getButton()->setText("Back");
			backButton->getTransform()->setPosition({ w / 2, h / 2 + 128 });

			pUIManager->controlsBackButtonId = backButton->getId();
		}
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

	RKT_INFO("Shutting down...");

	delete mpPerformanceTracker;
	delete mpFrameTimer;

	mIsRunning = false;
	return mIsRunning;
}

void GameApp::update()
{
	RocketEngine->update();

#ifdef RKP_DEBUG  
	
#endif
}

void GameApp::render()
{
	RocketEngine->render();
}

void GameApp::onMessage(RKTEngine::Message& message)
{
	RKTEngine::MessageDispatcher dispatcher(message);
	dispatcher.dispatch<RKTEngine::KeyDownMessage>(RKT_BIND_MESSAGE_FN(GameApp::back));
	dispatcher.dispatch<RKTEngine::ExitMessage>(RKT_BIND_MESSAGE_FN(GameApp::quit));

/*#ifdef RKP_DEBUG  
	dispatcher.dispatch<RKTEngine::DebugMessage>(RKT_BIND_MESSAGE_FN(GameApp::debugControls));
#endif*/

	RocketEngine->onMessage(message);
}

GameManager* GameApp::getGameManager()
{
	return RocketEngine->getSceneManager()->findGameObjectInScene(mGameManagerId)->getNativeScript()->get<GameManager>();
}

double GameApp::getTime()
{
	return RKTEngine::EngineCore::getInstance()->getTime();
}

bool GameApp::back(RKTEngine::KeyDownMessage& msg)
{
	if (msg.getKeyCode() == KeyCode::Escape)
	{
		if (RocketEngine->getSceneManager()->isActiveScene("game"))
		{
			GameObjManager->findGameObject(mGameManagerId)->getScript<GameManager>()->onPlaySelected();
		}
		else if (RocketEngine->getSceneManager()->isActiveScene("controls"))
			RocketEngine->getSceneManager()->loadScene("main");
		else
			mIsRunning = false;
	}
	return true;
}

bool GameApp::quit(RKTEngine::ExitMessage& msg)
{
	mIsRunning = false;
	return true;
}