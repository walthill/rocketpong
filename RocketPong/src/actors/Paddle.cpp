#include "Paddle.h"
#include "RocketEngine.h"

Paddle::Paddle(float startSpeed, bool isP1) :
	mIsP1(isP1), mSpeed(startSpeed)
{
	mpGameObject = GameObjManager->createSprite("paddle");
	auto spr = mpGameObject->getSprite();
	GameObjManager->addBoxCollider(mpGameObject->getId(), spr->getData()->mWidth, spr->getData()->mHeight);
	Actor::init();
}

Paddle::~Paddle()
{
}

bool Paddle::update(RKTEngine::UpdateMessage& message)
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

	return Actor::update(message);
}

void Paddle::onMessage(RKTEngine::Message& message)
{
	RKTEngine::MessageDispatcher dispatcher(message);
	dispatcher.dispatch<RKTEngine::UpdateMessage>(RKT_BIND_MESSAGE_FN(Paddle::update));
}