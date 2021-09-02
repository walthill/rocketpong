#ifndef ACTOR_H
#define ACTOR_H

#define RocketEngine RKTEngine::EngineCore::getInstance()

#include <RKTUtils/Trackable.h>
#include "RocketEngine.h"

class Actor : public RKTUtil::Trackable
{
	public:
		virtual bool update(RKTEngine::UpdateMessage& message) { return true; };
		virtual void onMessage(RKTEngine::Message& message) { mpGameObject->onMessage(message);  };

		RKTEngine::GameObject* mpGameObject = nullptr;

	protected:
		Actor() : mpGameObject(nullptr) {};
		~Actor() { if(mpGameObject != nullptr) RocketEngine->getEntityManager()->destroy(mpGameObject->getId()); };

};

#endif // !ACTOR_H
