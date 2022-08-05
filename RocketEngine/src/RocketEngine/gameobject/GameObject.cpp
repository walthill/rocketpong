#include "GameObject.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/component/TransformComponent.h"
#include "RocketEngine/core/ComponentManager.h"
#include "RocketEngine/actor/Actor.h"
#include <string>

namespace RKTEngine
{

	GameObject::GameObject() :
		name("")
	{
	}

	GameObject::~GameObject()
	{
		cleanup();
	}


	void GameObject::cleanup()
	{
	}

	void GameObject::update(float elapsedTime)
	{
		if (mpTransform->hasChanged())
		{
			SpriteComponent* spriteComponent = getSprite();
			if (spriteComponent != nullptr)
			{
				TransformData data = mpTransform->getData();
				spriteComponent->process(data.mPosition, data.mScale, data.mRotation.angle);
			}

			TextComponent* textComponent = getUILabel();
			if (textComponent != nullptr)
			{
				TransformData data = mpTransform->getData();
				textComponent->process(data.mPosition, data.mScale, data.mRotation.angle);
			}

			mpTransform->setHasChanged(false);
		}
	}

	void GameObject::setName()
	{ 
		name = "New GameObject("; 
		name.append(std::to_string(mId));
		name.append(")");
	}


	SpriteComponent* GameObject::getSprite()
	{
		auto pComponent = EngineCore::getInstance()->getComponentManager()->getSpriteComponent(mSpriteId);
		return pComponent;
	}

	BoxColliderComponent* GameObject::getBoxCollider()
	{
		auto pComponent = EngineCore::getInstance()->getComponentManager()->getBoxColliderComponent(mColliderId);
		return pComponent;
	}

	TextComponent* GameObject::getUILabel()
	{
		auto pComponent = EngineCore::getInstance()->getComponentManager()->getTextComponent(mLabelId);
		return pComponent;
	}

	AudioSourceComponent* GameObject::getAudioSource()
	{
		auto pComponent = EngineCore::getInstance()->getComponentManager()->getAudioSourceComponent(mAudioSourceId);
		return pComponent;
	}

	NativeScriptComponent* GameObject::getScript()
	{
		auto pComponent = EngineCore::getInstance()->getComponentManager()->getNativeScriptComponent(mNativeScriptId);
		return pComponent;
	}
}