#include "GameObject.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/core/ComponentManager.h"
#include "RocketEngine/gameobject/GameObjectManager.h"
#include <string>
#include "cereal/archives/json.hpp"


namespace RKTEngine
{

	GameObject::GameObject() :
		name("")
	{
		//register with ecs
		auto obj = EngineCore::getInstance()->getEntityManager()->createGameObject();
		mId = obj->getId();

		//create a transform when creating gameObjs outside the ecs
		if (getTransform() == nullptr)
		{
			auto pComponentManager = EngineCore::getInstance()->getComponentManager();
			connectTransform(pComponentManager->allocateTransformComponent());
			setTransformHandle(pComponentManager->getTransformComponent(mTransformId));
		}
	}

	GameObject::GameObject(GameObjectId id) :
		name(""), mId(id)
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

	void GameObject::addSpriteComponent(SpriteComponentData data)
	{
		EngineCore::getInstance()->getEntityManager()->addSprite(mId, data.mSpriteName, data.mTileName, data.mColor);
	}
	
	void GameObject::addBoxColliderComponent(BoxColliderData data)
	{
		EngineCore::getInstance()->getEntityManager()->addBoxCollider(mId, data.width, data.height, data.tag);
	}

	void GameObject::addAudioSourceComponent(AudioSourceComponentData data)
	{
		EngineCore::getInstance()->getEntityManager()->addAudioSource(mId, data.mAudioFileName);
	}

	void GameObject::addUILabelComponent(TextData data)
	{
		EngineCore::getInstance()->getEntityManager()->addUILabel(mId, data.mFontName, data.mText, data.mFontSize, data.mColor);
	}

	void GameObject::addNativeScriptComponent()
	{
		EngineCore::getInstance()->getEntityManager()->addNativeScript(mId);
	}


	SpriteComponent* GameObject::getSprite_Serialize() const
	{
		auto pComponent = EngineCore::getInstance()->getComponentManager()->getSpriteComponent(mSpriteId);
		return pComponent;
	}

	BoxColliderComponent* RKTEngine::GameObject::getBoxCollider_Serialize() const
	{
		auto pComponent = EngineCore::getInstance()->getComponentManager()->getBoxColliderComponent(mColliderId);
		return pComponent;
	}

	AudioSourceComponent* GameObject::getAudioSource_Serialize() const
	{
		auto pComponent = EngineCore::getInstance()->getComponentManager()->getAudioSourceComponent(mAudioSourceId);
		return pComponent;
	}

	TextComponent* GameObject::getUILabel_Serialize() const
	{
		auto pComponent = EngineCore::getInstance()->getComponentManager()->getTextComponent(mLabelId);
		return pComponent;
	}
	NativeScriptComponent* GameObject::getScript_Serialize() const
	{
		auto pComponent = EngineCore::getInstance()->getComponentManager()->getNativeScriptComponent(mNativeScriptId);
		return pComponent;
	}
}