#ifndef ACTOR_H
#define ACTOR_H

#define RocketEngine RKTEngine::EngineCore::getInstance()
#define GameObjManager RocketEngine->getEntityManager()
#define MSG_COMPLETE return true
#define DESERIALIZE(classname) void classname::onDeserialize(int id){auto obj = GameObjManager->findGameObject(id); if (obj != nullptr) obj->getNativeScript()->bind<classname>(id);}
//Attachs actor to serialization engine.
#define REGISTER_ACTOR(x)  CEREAL_REGISTER_TYPE(x); CEREAL_REGISTER_POLYMORPHIC_RELATION(RKTEngine::Actor, x); DESERIALIZE(x);
#define CEREAL_ACTOR() cereal::base_class<RKTEngine::Actor>(this)
#define CEREAL_BASE_CLASS(classname) cereal::base_class<classname>(this)

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
			bool isEnabled = true;

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
			
			uint32 gameObjectId = 0;
		private:
			friend class ComponentManager;
			friend class Serialization;
			friend class NativeScriptComponent;
			bool mFirstUpdate = true;

			friend cereal::access;
			template<class Archive>
			void serialize(Archive& ar)
			{
				ar(CEREAL_NVP(isEnabled));
			}
			virtual void onDeserialize(int id) {};

	};
}

#endif // !ACTOR_H
