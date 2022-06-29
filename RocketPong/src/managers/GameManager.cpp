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
		auto uiLabel = RocketEngine->getSceneManager()->findUILabelInScene("p1score");
		uiLabel->setText(mP1Score);
	}
	else
	{
		mP2Score++;
		auto uiLabel = RocketEngine->getSceneManager()->findUILabelInScene("p2score");
		uiLabel->setText(mP2Score);
	}
}

void GameManager::reset()
{
	mP1Score = 0;
	mP2Score = 0;
}
