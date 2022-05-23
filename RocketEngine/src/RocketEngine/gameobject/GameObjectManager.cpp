#include "GameObjectManager.h"
#include "GameObject.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/render/shader/ShaderManager.h"

namespace RKTEngine
{

	GameObjectId GameObjectManager::msNextUnitId = PLAYER_OBJ_ID + 1;

	GameObjectManager::GameObjectManager(uint32 maxSize) :
		mGameObjectPool(maxSize, sizeof(GameObject))
	{
	}

	GameObjectManager::~GameObjectManager()
	{
	}

	GameObject* GameObjectManager::createGameObject(const TransformData& transform, const SpriteComponentData& spriteData,
													const TextData& labelData, const GameObjectId& id)
	{
		GameObject* newObj = nullptr;

		RKTUtil::Byte* ptr = mGameObjectPool.allocateObject();

		if (ptr != nullptr)
		{
			newObj = ::new (ptr)GameObject();//placement new

			GameObjectId newId = id;
			if (newId == INVALID_GAMEOBJECT_ID) //new game obj
			{
				newId = msNextUnitId;
				msNextUnitId++;
			}

			//add new object to map and set the object's id locally
			mGameObjMap[newId] = newObj;
			newObj->setId(newId);

			//Hook up components
			ComponentManager* pComponentManager = EngineCore::getInstance()->getComponentManager();

			//TRANSFORM
			ComponentId newTransformId = pComponentManager->allocateTransformComponent(transform);
			newObj->connectTransform(newTransformId);
			newObj->setTransformHandle(pComponentManager->getTransformComponent(newTransformId));

			//SPRITE
			//Create sprite component, store id in new object, and load the sprite to the component
			if (!spriteData.mSpriteName.empty())
			{
				ComponentId newSpriteId = pComponentManager->allocateSpriteComponent(newObj->getTransformId(), spriteData);
				newObj->connectSprite(newSpriteId);
			}

			//TEXT LABEL
			//Create text component, store id in new object, and load the text to the component
			if (!labelData.mFontName.empty())
			{
				ComponentId newLabelId = pComponentManager->allocateTextComponent(newObj->getTransformId(), labelData);
				newObj->connectLabel(newLabelId);
			}
		}
		return newObj;
	}

	//TODO: create atlased version that takes in w/h & tile names 
	GameObject* GameObjectManager::createSprite(const std::string& spriteToLoad, glm::vec2 position, glm::vec2 scale, float rotation)
	{
		TransformData transformData = TransformData(position,scale, rotation);
		SpriteComponentData spriteData = SpriteComponentData(spriteToLoad);

		return createGameObject(transformData, spriteData);
	}

	GameObject* GameObjectManager::createLabel(const std::string& text, glm::vec2 position, glm::vec2 scale, float rotation, 
											   const std::string& fontName, int fontSize)
	{
		TransformData transformData = TransformData(position, scale, rotation);
		TextData textData = TextData(fontName, text, fontSize);

		return createGameObject(transformData, ZERO_SPRITE_DATA, textData);
	}

	void GameObjectManager::addBoxCollider(int objId, int w, int h, const std::string& t)
	{
		auto it = mGameObjMap.find(objId);
		if (it != mGameObjMap.end())
		{
			BoxColliderData data = { w,h,t };
			ComponentManager* pComponentManager = EngineCore::getInstance()->getComponentManager();
			ComponentId newBoxColliderId = pComponentManager->allocateBoxColliderComponent(data);
			it->second->connectCollider(newBoxColliderId);
			it->second->getBoxCollider()->setTransformParent(it->second->getTransform());
		}
	}

	GameObject* GameObjectManager::createPlayer(const std::string& texture, glm::vec2 position, glm::vec2 scale, float rotation)
	{
		TransformData transformData = TransformData(position, scale, rotation);
		SpriteComponentData spriteData = SpriteComponentData(texture);

		return createGameObject(transformData, spriteData, ZERO_LABEL_DATA, PLAYER_OBJ_ID);
	}


	GameObject* GameObjectManager::findGameObject(const GameObjectId& id) const
	{
		auto it = mGameObjMap.find(id);

		if (it != mGameObjMap.end())//found?
			return it->second;
		else
			return nullptr;
	}

	void GameObjectManager::destroy(const GameObjectId& id)
	{
		auto it = mGameObjMap.find(id);
		if (it != mGameObjMap.end())//found?
		{
			GameObject* obj = it->second;//hold for later

			//remove from map
			mGameObjMap.erase(it);

			//remove components from manager
			ComponentManager* pComponentManager = EngineCore::getInstance()->getComponentManager();
			pComponentManager->deallocateTransformComponent(obj->getTransformId());
			pComponentManager->deallocateSpriteComponent(obj->getSpriteId());
			pComponentManager->deallocateTextComponent(obj->getLabelId());
			pComponentManager->deallocateBoxColliderComponent(obj->getColliderId());

			//call destructor on gameObj
			obj->~GameObject();

			//free the object in the pool
			mGameObjectPool.freeObject((RKTUtil::Byte*)obj);
		}
	}

	void GameObjectManager::updateAll(float elapsedTime)
	{
		for (auto const& it : mGameObjMap)
		{
			it.second->update(elapsedTime);
		}
	}

	void GameObjectManager::onMessage(Message& message)
	{
		for (auto const& it : mGameObjMap)
			it.second->onMessage(message);
	}

	int GameObjectManager::getNumGameObjects()
	{
		return mGameObjMap.size();
	}
}