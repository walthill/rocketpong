#include "Actor.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/asset/scene/SceneManager.h"
#include "UILabel.h"

namespace RKTEngine
{
	void Actor::init(bool isUIElement)
	{
		setGameObjOwner();
		if (!isUIElement)
		{
			EngineCore::getInstance()->getSceneManager()->registerActor(this);
		}
		else
		{
			EngineCore::getInstance()->getSceneManager()->registerUI(dynamic_cast<UILabel*>(this));
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