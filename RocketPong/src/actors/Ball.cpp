#include "Ball.h"

Ball::Ball(glm::vec2 startPos, float speed) :
	mStartPos(startPos), mSpeed(speed)
{
	mpGameObject = GameObjManager->createSprite("ball");
	
	auto spr = mpGameObject->getSprite();
	GameObjManager->addBoxCollider(mpGameObject->getId(), spr->getData()->mWidth, spr->getData()->mHeight);

	getTransform()->setPosition(mStartPos);
	Actor::init();
}

Ball::~Ball()
{
}

bool Ball::onCollisionEnter(RKTEngine::CollisionEnterMessage& message)
{
	if (Actor::onCollisionEnter(message))
	{
		std::cout << "COLLISION BAYBEE" << std::endl;
	}

	return true;
}


bool Ball::update(RKTEngine::UpdateMessage& message)
{
	auto tr = getTransform();
	auto pos = tr->getPosition();
	pos += glm::vec2(-1, 0) * mSpeed * RocketEngine->sDeltaTime;
	tr->setPosition(pos);

	return Actor::update(message);
}

void Ball::onMessage(RKTEngine::Message& message)
{
	RKTEngine::MessageDispatcher dispatcher(message);
	dispatcher.dispatch<RKTEngine::UpdateMessage>(RKT_BIND_MESSAGE_FN(Ball::update));
	dispatcher.dispatch<RKTEngine::CollisionEnterMessage>(RKT_BIND_MESSAGE_FN(Ball::onCollisionEnter));
}
