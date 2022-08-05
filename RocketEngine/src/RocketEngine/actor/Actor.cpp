#include "Actor.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/gameobject/GameObjectManager.h"

namespace RKTEngine
{
	GameObject* Actor::getGameObject()
	{
		return RocketEngine->getEntityManager()->findGameObject(gameObjectId);
	}
}

