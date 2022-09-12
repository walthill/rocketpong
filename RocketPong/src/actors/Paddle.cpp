#include "Paddle.h"

void Paddle::onCreate()
{
	mSpeed = 300;
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
	}

	if (transform->getPosition().y > RocketEngine->getWindowHeight() || transform->getPosition().y < 0)
	{
		transform->setPosition(oldPos);
	}
}

#pragma region Serialization

void Paddle::onDeserialize(int id)
{
	auto obj = GameObjManager->findGameObject(id);
	if (obj != nullptr)
	{
		auto script = obj->getNativeScript();
		script->bind<Paddle>(id);
		auto inst = script->get<Paddle>();
		
		//assign vars
		inst->mIsP1 = mIsP1;
		inst->mSpeed = mSpeed;
	}
}

#pragma endregion
