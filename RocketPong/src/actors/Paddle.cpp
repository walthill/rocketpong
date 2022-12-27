#include "Paddle.h"
#include "GameApp.h"
#include "managers/GameManager.h"
#include "Ball.h"

REGISTER_ACTOR(Paddle);

void Paddle::onCreate()
{
	mSpeed = 300;
	mSprintScaler = 2.5f;
}

void Paddle::onStart()
{
	if (!mIsP1)
	{
		mHasAIInput = true;
	}
}

void Paddle::onDestroy()
{
}

void Paddle::onUpdate()
{
	auto transform = getTransform();
	auto oldPos = transform->getPosition();

	if (mIsP1)
	{
		if (Input::getKeyDown(KeyCode::W))
		{
			auto pos = transform->getPosition();
			transform->setPosition(pos.x, pos.y - mSpeed * RocketEngine->sDeltaTime);
		}
		else if (Input::getKeyDown(KeyCode::S))
		{
			auto pos = transform->getPosition();
			transform->setPosition(pos.x, pos.y + mSpeed * RocketEngine->sDeltaTime);
		}
		
		if (!mSprinting && Input::getKeyDown(KeyCode::LeftShift))
		{
			mSpeed *= mSprintScaler;
			mSprinting = true;
		}
	}
	else
	{
		mHasPlayerInput = false;
		if (Input::getKeyDown(KeyCode::P))
		{
			auto pos = transform->getPosition();
			transform->setPosition(pos.x, pos.y - mSpeed * RocketEngine->sDeltaTime);
			mHasPlayerInput = true;
		}
		else if (Input::getKeyDown(KeyCode::L))
		{
			auto pos = transform->getPosition();
			transform->setPosition(pos.x, pos.y + mSpeed * RocketEngine->sDeltaTime);
			mHasPlayerInput = true;
		}

		if (!mSprinting && Input::getKeyDown(KeyCode::RightShift))
		{
			mSprinting = true;
			mSpeed *= mSprintScaler;
			mHasPlayerInput = true;
		}

		if (mHasAIInput)
		{
			UpdatePaddleAI(transform);
		}
		UpdatePlayerInputTimer();
	}

	if (transform->getPosition().y > RocketEngine->getWindowHeight() || transform->getPosition().y < 0)
	{
		transform->setPosition(oldPos);
	}
}

void Paddle::onMessage(RKTEngine::Message& message)
{
	RKTEngine::MessageDispatcher dispatcher(message);
	dispatcher.dispatch<RKTEngine::KeyUpMessage>(RKT_BIND_MESSAGE_FN(Paddle::onKeyUp));
}

void Paddle::UpdatePaddleAI(RKTEngine::TransformComponent* transform)
{
	if (!mIsP1)
	{
		auto pos = transform->getPosition();
		auto ballScript = GameApp::getInstance()->getGameManager()->getBall();
		auto ballPos = ballScript->getGameObject()->getTransform()->getPosition();

		auto aiMaxSpeed = ballScript->getSpeed() * mAISpeedPercent;

		float diffY = (ballPos.y - pos.y) * mAISpeed;
		if (diffY > aiMaxSpeed) diffY = aiMaxSpeed;
		if (diffY < -aiMaxSpeed) diffY = -aiMaxSpeed;

		transform->setPosition(pos.x, pos.y + diffY * RocketEngine->sDeltaTime);
	}
}

void Paddle::UpdatePlayerInputTimer()
{
	if (mHasPlayerInput)
	{
		mInputTimer.start();
		mHasAIInput = false;
	}

	if (mInputTimer.getTimeElapsedInSeconds() > mAIControlThreshold)
	{
		mHasAIInput = true;
	}
}


bool Paddle::onKeyUp(RKTEngine::KeyUpMessage& msg)
{
	if (mIsP1)
	{
		if (mSprinting && msg.getKeyCode() == KeyCode::LeftShift)
		{
			mSpeed /= mSprintScaler;
			mSprinting = false;
		}
	}
	else
	{
		if (mSprinting && msg.getKeyCode() == KeyCode::RightShift)
		{
			mSpeed /= mSprintScaler;
			mSprinting = false;
		}
	}
	return true;
}
