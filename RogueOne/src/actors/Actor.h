#ifndef ACTOR_H
#define ACTOR_H

#define RocketEngine RKTEngine::EngineCore::getInstance()

#include <RKTUtils/Trackable.h>
#include "RocketEngine/input/MessageDefines.h"
#include "RocketEngine/gameobject/GameObject.h"

class Actor : public RKTUtil::Trackable
{
	public:
		virtual bool update(RKTEngine::UpdateMessage& message) { return true; };
		virtual void onMessage(RKTEngine::Message& message) { mpGameObject->onMessage(message);  };

		RKTEngine::GameObject* mpGameObject = nullptr;

	protected:
		Actor() : mpGameObject(nullptr) {};
		~Actor() { mpGameObject = nullptr; };

};

#endif // !ACTOR_H
