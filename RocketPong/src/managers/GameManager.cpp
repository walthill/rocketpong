#include "GameManager.h"
#include "RocketEngine.h"

GameManager::GameManager() : 
	mP1Score(0), mP2Score(0)
{
}

GameManager::~GameManager()
{
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
				label->setText(mP1Score);
		}
	}
}

void GameManager::reset()
{
	mP1Score = 0;
	mP2Score = 0;
}

void GameManager::setScoreUI(int p1ScoreId, int p2ScoreId)
{
	mP1ScoreId = p1ScoreId;
	mP2ScoreId = p2ScoreId;
}
