#include "Ball.h"

Ball::Ball(glm::vec2 startPos, float speed) :
	mStartPos(startPos), mSpeed(speed)
{
	mpGameObject = GameObjManager->createSprite("ball");
	
	auto spr = mpGameObject->getSprite();
	GameObjManager->addBoxCollider(mpGameObject->getId(), spr->getData()->mWidth/2, spr->getData()->mHeight/2);

	reset();
	Actor::init();
}

Ball::~Ball()
{
}

void Ball::reset()
{
	bool dir = Random::range(0, 1);
	mDir = dir == 0 ? glm::vec2(-1, 0) : glm::vec2(1, 0);

	mStartMoving = false;
	startTimer.start();
	getTransform()->setPosition(mStartPos);
}

bool Ball::onCollisionEnter(RKTEngine::CollisionEnterMessage& message)
{
	if (Actor::onCollisionEnter(message))
	{
		RKT_TRACE("COLLISION BAYBEE");
		
		auto other = message.otherCollider;
		if (other != nullptr)
		{
			if (other->getTag().compare("lp") == 0)
			{
				auto ballTr = getTransform();
				auto ballPos = getTransform()->getPosition();
				auto racketPos = other->getTransform()->getPosition();

				//help from https://www.noobtuts.com/cpp/2d-pong-game	
				mDir = glm::normalize(ballPos);
				auto ballMag = glm::length(ballPos);
				float t = ((ballPos.y - racketPos.y) / other->getHeight());
				mDir.x = 1;
				mDir.y = t;			}
			else if (other->getTag().compare("rp") == 0)
			{
				auto ballTr = getTransform();
				auto ballPos = getTransform()->getPosition();
				auto racketPos = other->getTransform()->getPosition();

				mDir = glm::normalize(ballPos);
				auto ballMag = glm::length(ballPos);
				float t = ((ballPos.y - racketPos.y) / other->getHeight());
				mDir.x = -1;
				mDir.y = t;
			}
		}
	}

	return true;
}


bool Ball::update(RKTEngine::UpdateMessage& message)
{
	//TODO: check x position to register goals

	if (!mStartMoving && startTimer.getTimeElapsedInSeconds() > mSTART_DELAY)
	{
		mStartMoving = true;
		startTimer.stop();
	}
	else if(mStartMoving)
	{

		auto tr = getTransform();
		auto pos = tr->getPosition();

		pos += mDir * mSpeed * RocketEngine->sDeltaTime;

		// wall hits
		if (pos.y > RocketEngine->getWindowHeight())
			mDir.y = -fabs(mDir.y);
		if (pos.y < 0)
			mDir.y = fabs(mDir.y);

		tr->setPosition(pos);
	}

	return Actor::update(message);
}

void Ball::onMessage(RKTEngine::Message& message)
{
	RKTEngine::MessageDispatcher dispatcher(message);
	dispatcher.dispatch<RKTEngine::UpdateMessage>(RKT_BIND_MESSAGE_FN(Ball::update));
	dispatcher.dispatch<RKTEngine::CollisionEnterMessage>(RKT_BIND_MESSAGE_FN(Ball::onCollisionEnter));
}
