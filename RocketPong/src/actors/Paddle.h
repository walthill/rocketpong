#ifndef PADDLE_H
#define PADDLE_H

#include "RocketEngine.h"

class Paddle : public RKTEngine::Actor
{
	public:
		
		virtual void onCreate() override;
		virtual void onStart() override;
		virtual void onDestroy() override;
		virtual void onUpdate() override;

		void onMessage(RKTEngine::Message& message);
		bool onKeyUp(RKTEngine::KeyUpMessage& msg);

		bool mIsP1;

	private:
		float mSpeed = 20;
		float mAISpeed = 4;
		float mAISpeedPercent = 0.4f;
		float mSprintScaler = 1.5f;
		int mAIControlThreshold = 15;
		bool mSprinting = false;
		bool mHasPlayerInput = true;
		bool mHasAIInput = false;
		RKTUtil::Timer mInputTimer;

		void UpdatePaddleAI(RKTEngine::TransformComponent* transform);
		void UpdatePlayerInputTimer();


	#pragma region Serialization

		friend cereal::access;
		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(CEREAL_NVP(mSpeed), CEREAL_NVP(mSprintScaler), CEREAL_NVP(mIsP1), CEREAL_ACTOR());
		}
	
		virtual void onDeserialize(int id) override;

	#pragma endregion

};

#endif //!PADDLE_H