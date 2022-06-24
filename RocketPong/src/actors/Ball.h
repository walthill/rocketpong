#ifndef BALL_H
#define BALL_H

#include <RocketEngine.h>
#include <RKTUtils/Timer.h>

class Ball : public RKTEngine::Actor
{
	public:
		Ball(glm::vec2 startPos, float speed);
		~Ball();

		void reset();

		virtual bool Ball::onCollisionEnter(RKTEngine::CollisionEnterMessage& message) override;
		virtual bool update(RKTEngine::UpdateMessage& message) override;
		virtual void onMessage(RKTEngine::Message& message) override;

	private:
		const float mSTART_DELAY = 2.0f;

		glm::vec2 mStartPos;
		glm::vec2 mDir;
		float mSpeed;
		bool mStartMoving = false;
		RKTUtil::Timer startTimer;
};

#endif // !BALL_H
