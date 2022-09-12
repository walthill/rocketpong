#include "NativeScriptComponent.h"
#include "RocketEngine/actor/Actor.h"

namespace RKTEngine
{
	void NativeScriptComponent::setInstance(std::shared_ptr<Actor> actor, int id, bool enabled)
	{
		pInstance = actor;
		pInstance->gameObjectId = id;
		pInstance->onCreate();
		pInstance->isEnabled = enabled;
	}
}