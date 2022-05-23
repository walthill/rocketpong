#ifndef ACTOR_H
#define ACTOR_H

#define RocketEngine RKTEngine::EngineCore::getInstance()
#define GameObjManager RocketEngine->getEntityManager()
#define MSG_COMPLETE return true

#include <RocketEngine/component/TransformComponent.h>
#include "RocketEngine/gameobject/GameObject.h"
#include <RocketEngine/input/MessageDefines.h>

namespace RKTEngine
{
	class Actor : public RKTUtil::Trackable
	{
		public:
			void init() { setGameObjOwner(); };
		
			virtual bool onCollisionEnter(RKTEngine::CollisionEnterMessage& message) { return mpGameObject->getColliderId() == message.colliderID; };
			virtual bool update(UpdateMessage& message) { return true; };
			virtual void onMessage(Message& message) OVERRIDE_REQUIRED;

			inline void Actor::setGameObjOwner() { mpGameObject->setOwner(this); }
			inline TransformComponent* Actor::getTransform() { return mpGameObject->getTransform(); }

			GameObject* mpGameObject = nullptr;
		protected:
			Actor() : mpGameObject(nullptr) {};
			~Actor() {};
	};
}

#endif // !ACTOR_H
