#include "GameManager.h"
#include "RocketEngine.h"
#include "../GameApp.h"
#include "../actors/Ball.h"

REGISTER_ACTOR(GameManager);

GameManager::GameManager() : 
	mP1Score(0), mP2Score(0)
{
}

GameManager::~GameManager()
{
}

void GameManager::onStart()
{
	GameObjManager->setButtonCallback(resumeButtonId, RKT_BIND_CALLBACK_FN(GameManager::onPlaySelected));
	GameObjManager->setButtonCallback(quitButtonId, RKT_BIND_CALLBACK_FN(GameManager::onQuitSelected));
}

void GameManager::onCreate()
{
	GameApp::getInstance()->setGameManager(Actor::gameObjectId);
}

void GameManager::onUpdate()
{
	if (Input::getKeyDown(KeyCode::R))
	{
		reset();
	}
}

void GameManager::score(bool p1Scored)
{
	if (p1Scored)
	{
		mP1Score++;
		auto gameObj = GameObjManager->findGameObject(mP1ScoreId);
		if (gameObj)
		{
			auto label = gameObj->getUILabel();
			if (label)
				label->setText(mP1Score);
		}
	}
	else
	{
		mP2Score++;
		auto gameObj = GameObjManager->findGameObject(mP2ScoreId);
		if (gameObj)
		{
			auto label = gameObj->getUILabel();
			if (label)
				label->setText(mP2Score);
		}
	}
	
	getGameObject()->getAudioSource()->play();
}

bool GameManager::isGameOver()
{
	return mP1Score == mSCORE_TO_WIN || mP2Score == mSCORE_TO_WIN;
}

void GameManager::reset()
{
	auto ball = GameObjManager->findGameObject(mBallId);
	ball->getNativeScript()->get<Ball>()->reset();

	auto p1ScoreUI = GameObjManager->findGameObject(mP1ScoreId);
	p1ScoreUI->getUILabel()->setText(0);
	auto p2ScoreUI = GameObjManager->findGameObject(mP2ScoreId);
	p2ScoreUI->getUILabel()->setText(0);

	mP1Score = 0;
	mP2Score = 0;
}

Ball* GameManager::getBall()
{
	auto ballObj = GameObjManager->findGameObject(mBallId);
	if (ballObj != nullptr)
		return ballObj->getNativeScript()->get<Ball>();
	else
		return nullptr;
}

void GameManager::setScoreUI(int p1ScoreId, int p2ScoreId)
{
	mP1ScoreId = p1ScoreId;
	mP2ScoreId = p2ScoreId;
}

void GameManager::onPlaySelected()
{
	RocketEngine->sTimeScaleActive = !RocketEngine->sTimeScaleActive;
	auto pauseText = GameObjManager->findGameObject(pauseTextId);
	if (pauseText != nullptr)
	{
		auto label = pauseText->getUILabel();
		if (label != nullptr)
		{
			label->setEnabled(!label->isEnabled());
		}
	}

	auto gameObj = GameObjManager->findGameObject(mP2ScoreId);
	if (gameObj)
	{
		auto label = gameObj->getUILabel();
		if (label)
			label->setEnabled(!label->isEnabled());
	}

	gameObj = GameObjManager->findGameObject(mP1ScoreId);
	if (gameObj)
	{
		auto label = gameObj->getUILabel();
		if (label)
			label->setEnabled(!label->isEnabled());
	}

	auto playBtn = GameObjManager->findGameObject(resumeButtonId);
	if (playBtn != nullptr)
	{
		auto btn = playBtn->getButton();
		if (btn != nullptr)
		{
			btn->setEnabled(!btn->isEnabled());
		}
	}

	auto quitBtn = GameObjManager->findGameObject(quitButtonId);
	if (quitBtn != nullptr)
	{
		auto btn = quitBtn->getButton();
		if (btn != nullptr)
		{
			btn->setEnabled(!btn->isEnabled());
		}
	}
}

void GameManager::onQuitSelected()
{
	RocketEngine->sTimeScaleActive = true;
	RocketEngine->getSceneManager()->loadScene("main");
}

void GameManager::setBallHandle(int ballId)
{
	mBallId = ballId;
}
