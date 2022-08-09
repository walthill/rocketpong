#include "GameObjectManager.h"
#include "GameObject.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/asset/scene/SceneManager.h"
#include "RocketEngine/core/ComponentManager.h"
#include "RocketEngine/render/shader/ShaderManager.h"
#include "RocketEngine/component/NativeScriptComponent.h"

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
			GameObjectId newId = id;
			if (newId == INVALID_GAMEOBJECT_ID) //new game obj
			{
				newId = msNextUnitId;
				msNextUnitId++;
			}
			newObj = ::new (ptr)GameObject(newId);//placement new

			//add new object to map and set the object's id locally
			mGameObjMap[newId] = newObj;
			newObj->setName();

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
				ComponentId newSpriteId = pComponentManager->allocateSpriteComponent(spriteData);
				newObj->connectSprite(newSpriteId);
			}

			//TEXT LABEL
			//Create text component, store id in new object, and load the text to the component
			if (!labelData.mFontName.empty())
			{
				ComponentId newLabelId = pComponentManager->allocateTextComponent(labelData);
				newObj->connectLabel(newLabelId);
			}

			EngineCore::getInstance()->getSceneManager()->registerEntity(newObj);
		}
		return newObj;
	}

	GameObject* GameObjectManager::registerGameObjectData(GameObject obj)
	{
		auto it = mGameObjMap.find(obj.getId());
		if (it != mGameObjMap.end())
		{
			auto newObj = it->second;
			newObj->name = obj.name;

			auto trId = obj.getTransformId();

			////Hook up components
			ComponentManager* pComponentManager = EngineCore::getInstance()->getComponentManager();
			newObj->connectTransform(trId);
			newObj->setTransformHandle(pComponentManager->getTransformComponent(trId));
		}

		return it->second;
	}

	GameObject* GameObjectManager::createActor(const TransformData& transform, const SpriteComponentData& spriteData,
		const TextData& labelData, const GameObjectId& id)
	{
		GameObject* newObj = nullptr;

		RKTUtil::Byte* ptr = mGameObjectPool.allocateObject();

		if (ptr != nullptr)
		{
			GameObjectId newId = id;
			if (newId == INVALID_GAMEOBJECT_ID) //new game obj
			{
				newId = msNextUnitId;
				msNextUnitId++;
			}
			newObj = ::new (ptr)GameObject(newId);//placement new

			//add new object to map and set the object's id locally
			mGameObjMap[newId] = newObj;
			newObj->setName();

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
				ComponentId newSpriteId = pComponentManager->allocateSpriteComponent(spriteData);
				newObj->connectSprite(newSpriteId);
			}

			//TEXT LABEL
			//Create text component, store id in new object, and load the text to the component
			if (!labelData.mFontName.empty())
			{
				ComponentId newLabelId = pComponentManager->allocateTextComponent(labelData);
				newObj->connectLabel(newLabelId);
			}

			addNativeScript(newObj->getId());

			EngineCore::getInstance()->getSceneManager()->registerEntity(newObj);
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


	void GameObjectManager::addAudioSource(int objId, const std::string& audio, float vol, float pan)
	{
		auto it = mGameObjMap.find(objId);
		if (it != mGameObjMap.end())
		{
			AudioSourceComponentData data = { audio, vol, pan };
			ComponentManager* pComponentManager = EngineCore::getInstance()->getComponentManager();
			ComponentId newAudioSrcID = pComponentManager->allocateAudioSourceComponent(data);
			it->second->connectAudioSource(newAudioSrcID);
		}
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

	void GameObjectManager::addSprite(int objId, const std::string& spriteToLoad, const std::string& tileName, Color color)
	{
		auto it = mGameObjMap.find(objId);
		if (it != mGameObjMap.end())
		{
			SpriteComponentData spriteData = SpriteComponentData(spriteToLoad, tileName, color);
			ComponentManager* pComponentManager = EngineCore::getInstance()->getComponentManager();
			ComponentId newSpriteId = pComponentManager->allocateSpriteComponent(spriteData);
			it->second->connectSprite(newSpriteId);
		}
	}

	void GameObjectManager::addUILabel(int objId, const std::string& font, const std::string& text, int size, Color color)
	{
		auto it = mGameObjMap.find(objId);
		if (it != mGameObjMap.end())
		{
			TextData data = TextData(font, text, size, color);
			ComponentManager* pComponentManager = EngineCore::getInstance()->getComponentManager();
			ComponentId id = pComponentManager->allocateTextComponent(data);
			it->second->connectLabel(id);
		}
	}

	void GameObjectManager::addNativeScript(int objId)
	{
		auto it = mGameObjMap.find(objId);
		if (it != mGameObjMap.end())
		{
			ComponentManager* pComponentManager = EngineCore::getInstance()->getComponentManager();
			ComponentId id = pComponentManager->allocateNativeScriptComponent(objId);
			it->second->connectNativeScript(id);
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

	int GameObjectManager::getNumGameObjects()
	{
		return mGameObjMap.size();
	}
}