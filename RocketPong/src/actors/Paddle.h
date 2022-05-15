#ifndef PADDLE_H
#define PADDLE_H

#include "RocketEngine.h"

class Paddle : public RKTEngine::Actor
{
	public:
		Paddle() {};
		Paddle(float startSpeed, bool isP1 = true);
		~Paddle();

		virtual bool update(RKTEngine::UpdateMessage& message) override;
		virtual void onMessage(RKTEngine::Message& message) override;

	private:
		bool mIsP1;
		float mSpeed = 20;
};

#endif //!PADDLE_H