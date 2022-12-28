#include "GameObject.h"
#include <string>
#include "cereal/archives/json.hpp"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/core/ComponentManager.h"
#include "RocketEngine/gameobject/GameObjectManager.h"

namespace RKTEngine
{

	GameObject::GameObject() :
		name("")
	{
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
		//if (mpTransform->hasChanged())	//allow for processing changes when transform hasn't been altered
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

			ButtonComponent* btnComponent = getButton();
			if (btnComponent != nullptr)
			{
				TransformData data = mpTransform->getData();
				btnComponent->processSprite(data.mPosition, data.mScale, data.mRotation.angle);
				btnComponent->processText(data.mPosition, data.mScale, data.mRotation.angle);
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

	void GameObject::onDeserialize(const TransformData& transformData)
	{
		//create version of this gameObj in the ecs system. Will be used to register deserialized components later in SceneManager.h
		EngineCore::getInstance()->getEntityManager()->createGameObject(transformData, mId);
		auto pComponentManager = EngineCore::getInstance()->getComponentManager();
		connectTransformId(pComponentManager->allocateTransformComponent(transformData));
		setTransformHandle(pComponentManager->getTransformComponent(mTransformId));
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

	ButtonComponent* GameObject::getButton()
	{
		auto pComponent = EngineCore::getInstance()->getComponentManager()->getButtonComponent(mButtonId);
		return pComponent;
	}

	AudioSourceComponent* GameObject::getAudioSource()
	{
		auto pComponent = EngineCore::getInstance()->getComponentManager()->getAudioSourceComponent(mAudioSourceId);
		return pComponent;
	}

	NativeScriptComponent* GameObject::getNativeScript()
	{
		auto pComponent = EngineCore::getInstance()->getComponentManager()->getNativeScriptComponent(mNativeScriptId);
		return pComponent;
	}

	void GameObject::addSpriteComponent(const SpriteComponentData& data)
	{
		EngineCore::getInstance()->getEntityManager()->addSprite(mId, data.mSpriteName, data.mTileName, data.mColor, data.isEnabled);
	}
	
	void GameObject::addBoxColliderComponent(const BoxColliderData& data)
	{
		EngineCore::getInstance()->getEntityManager()->addBoxCollider(mId, data.width, data.height, data.tag, data.isEnabled);
	}

	void GameObject::addAudioSourceComponent(const AudioSourceComponentData& data)
	{
		EngineCore::getInstance()->getEntityManager()->addAudioSource(mId, data.mAudioFileName, data.isEnabled);
	}

	void GameObject::addUILabelComponent(const TextData& data)
	{
		EngineCore::getInstance()->getEntityManager()->addUILabel(mId, data.mFontName, data.mText, data.mFontSize, data.mColor, data.isEnabled);
	}

	void GameObject::addButtonComponent(const ButtonComponentData& data)
	{
		EngineCore::getInstance()->getEntityManager()->addButton(mId, data.mText.mFontName, data.mText.mText, data.mText.mFontSize, data.mText.mColor,
																 data.mSprite.mSpriteName, data.mSprite.mTileName, data.mSprite.mColor, 
																 data.mHighlightSprite.mSpriteName, data.mHighlightSprite.mTileName, data.mHighlightSprite.mColor, data.isEnabled);
	}

	void GameObject::addNativeScriptComponent(std::shared_ptr<Actor> actor)
	{
		EngineCore::getInstance()->getEntityManager()->addNativeScript(actor, mId, actor->isEnabled);
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
	ButtonComponent* GameObject::getButton_Serialize() const
	{
		auto pComponent = EngineCore::getInstance()->getComponentManager()->getButtonComponent(mButtonId);
		return pComponent;
	}
	NativeScriptComponent* GameObject::getScript_Serialize() const
	{
		auto pComponent = EngineCore::getInstance()->getComponentManager()->getNativeScriptComponent(mNativeScriptId);
		return pComponent;
	}
}