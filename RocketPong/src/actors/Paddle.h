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

		bool mIsP1;

	private:
		float mSpeed = 20;

	#pragma region Serialization

		friend cereal::access;
		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(CEREAL_NVP(mSpeed), CEREAL_NVP(mIsP1), CEREAL_ACTOR());
		}
	
		virtual void onDeserialize(int id) override;

	#pragma endregion

};

#endif //!PADDLE_H