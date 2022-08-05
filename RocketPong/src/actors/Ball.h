#ifndef BALL_H
#define BALL_H

#include <RocketEngine.h>
#include <RKTUtils/Timer.h>

class Ball : public RKTEngine::Actor
{
	public:		
		virtual void onCreate() override;
		virtual void onStart() override;
		//virtual void onDestroy();
		virtual void onUpdate() override;
		//virtual void onSerialize() override;
		//virtual void onDeserialize() override;

		void reset();

		virtual bool onCollisionEnter(RKTEngine::CollisionEnterMessage& message) override;
		virtual void onMessage(RKTEngine::Message& message) override;

		glm::vec2 mStartPos;
	
	private:
		const float mSTART_DELAY = 2.0f;

		glm::vec2 mDir;
		float mSpeed;
		bool mStartMoving = false;
		RKTUtil::Timer startTimer;
};

#endif // !BALL_H
