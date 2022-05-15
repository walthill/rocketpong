#include "Paddle.h"
#include "RocketEngine.h"

Paddle::Paddle(float startSpeed, bool isP1) :
	mIsP1(isP1), mSpeed(startSpeed)
{
	mpGameObject = RocketEngine->getEntityManager()->createSprite("paddle");	
	Actor::init();
}

Paddle::~Paddle()
{
}

bool Paddle::update(RKTEngine::UpdateMessage& message)
{
	if (mIsP1)
	{
		if (Input::getKeyDown(KeyCode::W))
		{
			auto transform = getTransform();
			auto pos = transform->getPosition();
			transform->setPosition(pos.x, pos.y - mSpeed * RocketEngine->sDeltaTime);
		}
		else if (Input::getKeyDown(KeyCode::S))
		{
			auto transform = getTransform();
			auto pos = transform->getPosition();
			transform->setPosition(pos.x, pos.y + mSpeed * RocketEngine->sDeltaTime);
		}
	}
	else
	{
		if (Input::getKeyDown(KeyCode::P))
		{
			auto transform = getTransform();
			auto pos = transform->getPosition();
			transform->setPosition(pos.x, pos.y - mSpeed * RocketEngine->sDeltaTime);
		}
		else if (Input::getKeyDown(KeyCode::L))
		{
			auto transform = getTransform();
			auto pos = transform->getPosition();
			transform->setPosition(pos.x, pos.y + mSpeed * RocketEngine->sDeltaTime);
		}
	}

	return Actor::update(message);
}

void Paddle::onMessage(RKTEngine::Message& message)
{
	RKTEngine::MessageDispatcher dispatcher(message);
	dispatcher.dispatch<RKTEngine::UpdateMessage>(RKT_BIND_MESSAGE_FN(Paddle::update));
}