#include "GameManager.h"
#include "RocketEngine.h"
#include "../GameApp.h"
#include "../actors/Ball.h"

GameManager::GameManager() : 
	mP1Score(0), mP2Score(0)
{
}

GameManager::~GameManager()
{
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
		auto gameObj = RocketEngine->getSceneManager()->findGameObjectInScene(mP1ScoreId);
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
		auto gameObj = RocketEngine->getSceneManager()->findGameObjectInScene(mP2ScoreId);
		if (gameObj)
		{
			auto label = gameObj->getUILabel();
			if (label)
				label->setText(mP2Score);
		}
	}
}

void GameManager::reset()
{
	auto ball = RocketEngine->getSceneManager()->findGameObjectInScene(mBallId);
	ball->getNativeScript()->get<Ball>()->reset();

	auto p1ScoreUI = RocketEngine->getSceneManager()->findGameObjectInScene(mP1ScoreId);
	p1ScoreUI->getUILabel()->setText(0);
	auto p2ScoreUI = RocketEngine->getSceneManager()->findGameObjectInScene(mP2ScoreId);
	p2ScoreUI->getUILabel()->setText(0);

	mP1Score = 0;
	mP2Score = 0;
}

void GameManager::setScoreUI(int p1ScoreId, int p2ScoreId)
{
	mP1ScoreId = p1ScoreId;
	mP2ScoreId = p2ScoreId;
}

void GameManager::setBallHandle(int ballId)
{
	mBallId = ballId;
}


void GameManager::onDeserialize(int id)
{
	auto obj = GameObjManager->findGameObject(id);
	if (obj != nullptr)
	{
		auto script = obj->getNativeScript();
		script->bind<GameManager>(id);
		auto inst = script->get<GameManager>();

		//restore serialized values here
		inst->mP1ScoreId = mP1ScoreId;
		inst->mP2ScoreId = mP2ScoreId;
		inst->mBallId = mBallId;
	}
}
