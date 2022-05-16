#ifndef BALL_H
#define BALL_H

#include <RocketEngine.h>

class Ball : public RKTEngine::Actor
{
	public:
		Ball(glm::vec2 startPos, float speed);
		~Ball();

		virtual bool Ball::onCollisionEnter(RKTEngine::CollisionEnterMessage& message) override;
		virtual bool update(RKTEngine::UpdateMessage& message) override;
		virtual void onMessage(RKTEngine::Message& message) override;

	private:
		glm::vec2 mStartPos;
		float mSpeed;
};

#endif // !BALL_H
