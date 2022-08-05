#ifndef PADDLE_H
#define PADDLE_H

#include "RocketEngine.h"

class Paddle : public RKTEngine::Actor
{
	public:
		
		virtual void onCreate() override;
		virtual void onStart() override;
		//virtual void onDestroy();
		virtual void onUpdate() override;
		//virtual void onSerialize() override;
		//virtual void onDeserialize() override;

		bool mIsP1;

	private:
		float mSpeed = 20;
};

#endif //!PADDLE_H