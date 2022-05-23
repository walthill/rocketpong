#include "GameObject.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/component/TransformComponent.h"
#include "RocketEngine/core/ComponentManager.h"
#include "RocketEngine/actor/Actor.h"

namespace RKTEngine
{

	GameObject::GameObject()
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

	void GameObject::onMessage(Message& message)
	{
		if (mpActorOwner != nullptr)
			mpActorOwner->onMessage(message);
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
}