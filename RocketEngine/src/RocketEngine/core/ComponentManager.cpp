#include "ComponentManager.h"
#include "RocketEngine/render/shader/ShaderManager.h"
#include "RocketEngine/input/message/CollisionEnterMessage.h"
#include <RocketEngine\core\MessageManager.h>
#include <RocketEngine\core\EngineCore.h>

namespace RKTEngine
{
	ComponentId ComponentManager::msNextMaterialComponentId = 0;
	ComponentId ComponentManager::msNextSpriteComponentId = 0;
	ComponentId ComponentManager::msNextTextComponentId = 0;
	ComponentId ComponentManager::msNextTransformComponentId = 0;
	ComponentId ComponentManager::msNextColliderComponentId = 0;

	ComponentManager::ComponentManager(uint32 maxSize)
		: mTransformPool(maxSize, sizeof(TransformComponent))
		, mLabelPool(maxSize, sizeof(TextComponent))
		, mSpritePool(maxSize, sizeof(SpriteComponent))
		, mColliderPool(maxSize, sizeof(BoxColliderComponent))
	{
	}

	ComponentManager::~ComponentManager()
	{
		clean();
	}

	void ComponentManager::clean()
	{
		//call destructor for all components
		for (auto& it : mTransformComponentMap)
		{
			TransformComponent* pComponent = it.second;
			pComponent->~TransformComponent();
		}
		for (auto& it : mSpriteComponentMap)
		{
			SpriteComponent* pComponent = it.second;
			pComponent->~SpriteComponent();
		}
		for (auto& it : mTextComponentMap)
		{
			TextComponent* pComponent = it.second;
			pComponent->~TextComponent();
		}
		for (auto& it : mColliderComponentMap)
		{
			BoxColliderComponent* pComponent = it.second;
			pComponent->~BoxColliderComponent();
		}

		//clear maps
		mTransformComponentMap.clear();
		mSpriteComponentMap.clear();
		mTextComponentMap.clear();
		mColliderComponentMap.clear();

		//reset memory pools
		mTransformPool.reset();
		mSpritePool.reset();
		mLabelPool.reset();
		mColliderPool.reset();
	}


	/******************************************************************************
	******************************************************************************

		TRANSFORM COMPONENT

	******************************************************************************
	*****************************************************************************/


	TransformComponent* ComponentManager::getTransformComponent(const ComponentId& id)
	{
		auto& it = mTransformComponentMap.find(id);

		if (it != mTransformComponentMap.end())
			return it->second;
		else
			return nullptr;
	}

	ComponentId ComponentManager::allocateTransformComponent(const TransformData& data)
	{
		ComponentId newID = INVALID_COMPONENT_ID;

		RKTUtil::Byte* ptr = mTransformPool.allocateObject();
		if (ptr != nullptr)
		{
			newID = msNextTransformComponentId;
			TransformComponent* pComponent = ::new (ptr)TransformComponent(newID); //placement new https://stackoverflow.com/questions/19831298/operator-new-function-does-not-take-2-arguments
			pComponent->setData(data);
			mTransformComponentMap[newID] = pComponent;
			msNextTransformComponentId++;//increment id
		}

		return newID;
	}

	void ComponentManager::deallocateTransformComponent(const ComponentId& id)
	{
		auto it = mTransformComponentMap.find(id);

		if (it != mTransformComponentMap.end())//found it
		{
			TransformComponent* ptr = it->second;
			mTransformComponentMap.erase(it);

			ptr->~TransformComponent();
			mTransformPool.freeObject((RKTUtil::Byte*)ptr);
		}
	}

	/******************************************************************************
	******************************************************************************

		SPRITE COMPONENT

	******************************************************************************
	*****************************************************************************/


	SpriteComponent* ComponentManager::getSpriteComponent(const ComponentId& id)
	{
		auto& it = mSpriteComponentMap.find(id);

		if (it != mSpriteComponentMap.end())
			return it->second;
		else
			return nullptr;
	}

	//Load model and assign to gameobject
	ComponentId ComponentManager::allocateSpriteComponent(const ComponentId& spriteID, const SpriteComponentData& data)
	{
		ComponentId newID = INVALID_COMPONENT_ID;
		RKTUtil::Byte* ptr = mSpritePool.allocateObject();

		if (ptr != nullptr)
		{
			newID = msNextSpriteComponentId;
			SpriteComponent* pComponent = ::new (ptr)SpriteComponent(newID);
			pComponent->setData(data);
			pComponent->load();			//load model mesh
			mSpriteComponentMap[newID] = pComponent;
			msNextSpriteComponentId++;//increment id
		}

		return newID;
	}

	void ComponentManager::deallocateSpriteComponent(const ComponentId& id)
	{
		auto it = mSpriteComponentMap.find(id);

		if (it != mSpriteComponentMap.end())//found it
		{
			SpriteComponent* ptr = it->second;
			mSpriteComponentMap.erase(it);

			ptr->~SpriteComponent();
			mSpritePool.freeObject((RKTUtil::Byte*)ptr);
		}
	}

	/******************************************************************************
	******************************************************************************

	TEXT LABEL COMPONENT

	******************************************************************************
	*****************************************************************************/


	TextComponent* ComponentManager::getTextComponent(const ComponentId& id)
	{
		auto& it = mTextComponentMap.find(id);

		if (it != mTextComponentMap.end())
			return it->second;
		else
			return nullptr;
	}

	ComponentId ComponentManager::allocateTextComponent(const ComponentId& labelID, const TextData& data)
	{
		ComponentId newID = INVALID_COMPONENT_ID;
		RKTUtil::Byte* ptr = mLabelPool.allocateObject();

		if (ptr != nullptr)
		{
			newID = msNextTextComponentId;
			TextComponent* pComponent = ::new (ptr)TextComponent(newID);
			pComponent->setData(data);
			pComponent->load();			
			mTextComponentMap[newID] = pComponent;
			msNextTextComponentId++;
		}

		return newID;
	}

	void ComponentManager::deallocateTextComponent(const ComponentId& id)
	{
		auto it = mTextComponentMap.find(id);

		if (it != mTextComponentMap.end())
		{
			TextComponent* ptr = it->second;
			mTextComponentMap.erase(it);

			ptr->~TextComponent();
			mLabelPool.freeObject((RKTUtil::Byte*)ptr);
		}
	}


	/******************************************************************************
	******************************************************************************

	BOX COLLIDER COMPONENT

	******************************************************************************
	*****************************************************************************/


	BoxColliderComponent* ComponentManager::getBoxColliderComponent(const ComponentId& id)
	{
		auto& it = mColliderComponentMap.find(id);

		if (it != mColliderComponentMap.end())
			return it->second;
		else
			return nullptr;
	}

	ComponentId ComponentManager::allocateBoxColliderComponent(const BoxColliderData& data)
	{
		ComponentId newID = INVALID_COMPONENT_ID;
		RKTUtil::Byte* ptr = mColliderPool.allocateObject();

		if (ptr != nullptr)
		{
			newID = msNextColliderComponentId;
			BoxColliderComponent* pComponent = ::new (ptr)BoxColliderComponent(newID);
			pComponent->setData(data);
			mColliderComponentMap[newID] = pComponent;
			msNextColliderComponentId++;
		}

		return newID;
	}

	void ComponentManager::deallocateBoxColliderComponent(const ComponentId& id)
	{
		auto it = mColliderComponentMap.find(id);

		if (it != mColliderComponentMap.end())
		{
			BoxColliderComponent* ptr = it->second;
			mColliderComponentMap.erase(it);

			ptr->~BoxColliderComponent();
			mColliderPool.freeObject((RKTUtil::Byte*)ptr);
		}
	}

	void ComponentManager::update(float elapsedTime)
	{
		//AABB testing
		for (auto& it : mColliderComponentMap)
		{
			auto collider = it.second;
			for (auto& compareIter : mColliderComponentMap)
			{
				auto otherCollider= compareIter.second;
				//prevent self-collisions
				if (collider->getId() == otherCollider->getId())
					continue;

				if (collider->checkCollision(otherCollider))
				{
					auto messageMan = EngineCore::getInstance()->getMessageManager();
					Message* pMessage = new CollisionEnterMessage(collider->getId());
					messageMan->addMessage(pMessage, 1);
					Message* pMessage2 = new CollisionEnterMessage(otherCollider->getId());
					messageMan->addMessage(pMessage2, 1);
				}
			}
		}
	}

	void ComponentManager::renderComponents()
	{
		renderSprites();
		renderText();
	}

	void ComponentManager::renderSprites()
	{
		for (auto& it : mSpriteComponentMap)
		{
			it.second->render();
		}
	}
	void ComponentManager::renderText()
	{
		for (auto& it : mTextComponentMap)
		{
			it.second->render();
		}
	}
	void ComponentManager::renderWireframes()
	{
		for (auto& it : mColliderComponentMap)
		{
			it.second->renderOverlay();
		}
	}
}