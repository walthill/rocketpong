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
			virtual ~Actor() {}

			inline TransformComponent* Actor::getTransform() { return getGameObject()->getTransform(); }
			GameObject* getGameObject();

		protected:
			//Object instatiated
			virtual void onCreate() {};
			//First update loop
			virtual void onStart() {};
			virtual void onDestroy() {};
			virtual void onUpdate() {};

			virtual void onMessage(Message& message) {};
			virtual bool onCollisionEnter(RKTEngine::CollisionEnterMessage& message) { return getGameObject()->getColliderId() == message.colliderID; };
			
			virtual void onSerialize() {};
			virtual void onDeserialize() {};

			uint32 gameObjectId;
		private:
			friend class ComponentManager;
			friend class Serialization;
			friend class NativeScriptComponent;
			bool mFirstUpdate = true;
	};
}

#endif // !ACTOR_H
