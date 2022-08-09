#include "Actor.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/gameobject/GameObjectManager.h"
#include "RocketEngine/gameobject/GameObject.h"
#include <RocketEngine/component/TransformComponent.h>

namespace RKTEngine
{
	TransformComponent* Actor::getTransform()
	{
		return getGameObject()->getTransform();
	}
	GameObject* Actor::getGameObject()
	{
		return RocketEngine->getEntityManager()->findGameObject(gameObjectId);
	}
	bool Actor::onCollisionEnter(RKTEngine::CollisionEnterMessage& message)
	{
		return getGameObject()->getColliderId() == message.colliderID;
	}
}

