#ifndef BALL_H
#define BALL_H

#include <RocketEngine.h>
#include <RKTUtils/Timer.h>

class Ball : public RKTEngine::Actor
{
	public:		
		virtual void onCreate() override;
		virtual void onStart() override;
		virtual void onUpdate() override;

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

	//Save & load Actor variables here.
	#pragma region Serialization

		friend cereal::access;
		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(CEREAL_NVP(mSpeed), CEREAL_NVP(mStartPos), CEREAL_ACTOR());
		}

		void onDeserialize(int id) override;

	#pragma endregion
};


#endif // !BALL_H
