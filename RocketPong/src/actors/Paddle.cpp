#include "Paddle.h"

REGISTER_ACTOR(Paddle);

void Paddle::onCreate()
{
	mSpeed = 300;
	mSprintScaler = 2.5f;
}

void Paddle::onStart()
{
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
		if (Input::getKeyDown(KeyCode::P))
		{
			auto pos = transform->getPosition();
			transform->setPosition(pos.x, pos.y - mSpeed * RocketEngine->sDeltaTime);
		}
		else if (Input::getKeyDown(KeyCode::L))
		{
			auto pos = transform->getPosition();
			transform->setPosition(pos.x, pos.y + mSpeed * RocketEngine->sDeltaTime);
		}

		if (!mSprinting && Input::getKeyDown(KeyCode::RightShift))
		{
			mSprinting = true;
			mSpeed *= mSprintScaler;
		}

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
