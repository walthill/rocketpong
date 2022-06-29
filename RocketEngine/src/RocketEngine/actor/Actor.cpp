#include "Actor.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/asset/scene/SceneManager.h"

namespace RKTEngine
{
	void Actor::init(bool isUIElement)
	{
		setGameObjOwner();
		if (!isUIElement)
		{
			EngineCore::getInstance()->getSceneManager()->registerEntity(this);
		}
	}

	Actor::~Actor()
	{
		if (mpGameObject != nullptr)
		{
			EngineCore::getInstance()->getEntityManager()->destroy(mpGameObject->getId());
		}
	}
}