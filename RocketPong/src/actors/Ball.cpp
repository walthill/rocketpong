#include "Ball.h"
#include "GameApp.h"
#include "managers/GameManager.h"
#include "RocketEngine.h"

REGISTER_ACTOR(Ball); 

void Ball::onCreate()
{
}

void Ball::onStart()
{
	reset();
}

void Ball::onUpdate()
{
	if (!mStartMoving && startTimer.getTimeElapsedInSeconds() > mSTART_DELAY)
	{
		if(GameApp::getInstance()->getGameManager()->isGameOver())
			RocketEngine->getSceneManager()->loadScene("main");

		mStartMoving = true;
		startTimer.stop();
	}
	else if (mStartMoving)
	{
		auto tr = getTransform();
		auto pos = tr->getPosition();

		pos += mDir * mSpeed * RocketEngine->sDeltaTime;

		// wall hits
		if (pos.y > RocketEngine->getWindowHeight())
			mDir.y = -fabs(mDir.y);
		else if (pos.y < 0)
			mDir.y = fabs(mDir.y);

		tr->setPosition(pos);

		//goals
		if (pos.x > RocketEngine->getWindowWidth())
		{
			GameApp::getInstance()->getGameManager()->score(true);
			reset();
		}
		else if (pos.x < 0)
		{
			GameApp::getInstance()->getGameManager()->score(false);
			reset();
		}
	}
}

bool Ball::onCollisionEnter(RKTEngine::CollisionEnterMessage& message)
{
	if (Actor::onCollisionEnter(message))
	{
		auto other = message.otherCollider;
		if (other != nullptr)
		{
			if (other->getTag().compare("lp") == 0)
			{
				if (!mP1LastPaddleHit)
				{
					mP1LastPaddleHit = true;
					mVolleyCounter += 1;
				}
				else
				{
					mVolleyCounter = 0;
				}

				if (mVolleyCounter != 0 && mVolleyCounter % mVolleyBoostThreshold == 0)
				{
					mSpeed *= mSpeedScaler;
				}

				auto ballTr = getTransform();
				auto ballPos = getTransform()->getPosition();
				auto racketPos = other->getTransform()->getPosition();

				//help from https://www.noobtuts.com/cpp/2d-pong-game	
				mDir = glm::normalize(ballPos);
				auto ballMag = glm::length(ballPos);
				float t = ((ballPos.y - racketPos.y) / other->getHeight());
				mDir.x = 1;
				mDir.y = t;

				getGameObject()->getAudioSource()->play();
			}
			else if (other->getTag().compare("rp") == 0)
			{
				if (mP1LastPaddleHit)
				{
					mP1LastPaddleHit = false;
					mVolleyCounter += 1;
				}
				else
				{
					mVolleyCounter = 0;
				}

				if (mVolleyCounter != 0 && mVolleyCounter % mVolleyBoostThreshold == 0)
				{
					mSpeed *= mSpeedScaler;
				}

				auto ballTr = getTransform();
				auto ballPos = getTransform()->getPosition();
				auto racketPos = other->getTransform()->getPosition();

				mDir = glm::normalize(ballPos);
				auto ballMag = glm::length(ballPos);
				float t = ((ballPos.y - racketPos.y) / other->getHeight());
				mDir.x = -1;
				mDir.y = t;

				getGameObject()->getAudioSource()->play();
			}
		}
	}

	return true;
}

void Ball::onMessage(RKTEngine::Message& message)
{
	RKTEngine::MessageDispatcher dispatcher(message);
	dispatcher.dispatch<RKTEngine::CollisionEnterMessage>(RKT_BIND_MESSAGE_FN(Ball::onCollisionEnter));
}

void Ball::reset()
{
	mSpeed = 350;
	mSpeedScaler = 1.5f;
	mVolleyBoostThreshold = 6;
	mVolleyCounter = 0;
	
	float dir = Random::range_real(0.45f, 0.55f);
	float angle = 360 * dir;
	angle = glm::radians(angle);
	mDir = glm::vec2(cosf(angle), sinf(angle));
	mDir = glm::normalize(mDir);
	
	mStartMoving = false;
	startTimer.start();
	auto transform = getTransform();
	if(transform != nullptr)
		transform->setPosition(mStartPos);
}
