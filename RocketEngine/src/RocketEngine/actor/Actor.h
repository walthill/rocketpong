#ifndef ACTOR_H
#define ACTOR_H

#define RocketEngine RKTEngine::EngineCore::getInstance()
#define GameObjManager RocketEngine->getEntityManager()
#define MSG_COMPLETE return true
#define REGISTER_ACTOR(x) CEREAL_REGISTER_TYPE(x); CEREAL_REGISTER_POLYMORPHIC_RELATION(RKTEngine::Actor, x)

#include <RKTUtils/Trackable.h>
#include <RocketEngine/input/MessageDefines.h>
#include <cereal/access.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>

namespace RKTEngine
{
	class GameObject;
	class TransformComponent;

	class Actor : public RKTUtil::Trackable
	{
		public:
			virtual ~Actor() {};

			TransformComponent* Actor::getTransform();
			GameObject* getGameObject();

		protected:
			friend GameObject;

			//Object instatiated
			virtual void onCreate() {};
			//First update loop
			virtual void onStart() {};
			virtual void onDestroy() {};
			virtual void onUpdate() {};

			virtual void onMessage(Message& message) {};
			virtual bool onCollisionEnter(RKTEngine::CollisionEnterMessage& message);
			
			virtual void onDeserialize(int id) {};
			
			uint32 gameObjectId;
		private:
			friend class ComponentManager;
			friend class Serialization;
			friend class NativeScriptComponent;
			bool mFirstUpdate = true;

			friend cereal::access;
			template<class Archive>
			void serialize(Archive& archive)
			{
			}

	};
}

#endif // !ACTOR_H
