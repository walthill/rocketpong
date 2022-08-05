#include "Paddle.h"
#include "RocketEngine.h"

void Paddle::onCreate()
{
	auto gameObj = getGameObject();
	GameObjManager->addSprite(gameObjectId, "paddle");
	auto spr = gameObj->getSprite();
	GameObjManager->addBoxCollider(gameObjectId, spr->getData()->mWidth, spr->getData()->mHeight);
}

void Paddle::onStart()
{
	mSpeed = 300;
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