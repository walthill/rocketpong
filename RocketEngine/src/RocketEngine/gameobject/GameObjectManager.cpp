#include "GameObjectManager.h"
#include "GameObject.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/core/Log.h"
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
		mGameObjMap = std::map<GameObjectId, GameObject*>();
	}

	GameObjectManager::~GameObjectManager()
	{
	}

	GameObject* GameObjectManager::createGameObject(const TransformData& transform, const SpriteComponentData& spriteData,
													const TextData& labelData, const ButtonComponentData& btnData, const GameObjectId& id)
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
			newObj->connectTransformId(newTransformId);
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

			//BUTTON 
			//Create button component, store id in new object, and load the text to the component
			if (!btnData.mSprite.mSpriteName.empty() && !btnData.mHighlightSprite.mSpriteName.empty() 
				&& !btnData.mText.mFontName.empty())
			{
				ComponentId id = pComponentManager->allocateButtonComponent(btnData);
				newObj->connectButton(id);
			}

			EngineCore::getInstance()->getSceneManager()->registerEntity(newObj);
		}
		return newObj;
	}

	GameObject* GameObjectManager::createGameObject(const TransformData& transform, const GameObjectId& id)
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
			newObj->connectTransformId(newTransformId);
			newObj->setTransformHandle(pComponentManager->getTransformComponent(newTransformId));

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

			auto trId = newObj->getTransformId();

			////Hook up components
			ComponentManager* pComponentManager = EngineCore::getInstance()->getComponentManager();
			//newObj->getTransform()->setData(obj.getTransform()->getData());			
			//newObj->connectTransformId(trId);
			//newObj->setTransformHandle(pComponentManager->getTransformComponent(trId));

			auto box = newObj->getBoxCollider();
			if (box)
			{
				box->setTransformParent(pComponentManager->getTransformComponent(trId));
			}
		}

		return it->second;
	}

	GameObject* GameObjectManager::createActor(const TransformData& transform, const SpriteComponentData& spriteData,
		const TextData& labelData, const ButtonComponentData& btnData, const GameObjectId& id)
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
			newObj->connectTransformId(newTransformId);
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

			addNativeScript(nullptr, newObj->getId());

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

	GameObject* GameObjectManager::createButton(const std::string & spr, const std::string & highlightSpr, glm::vec2 pos, glm::vec2 scale, float rot,
												const std::string& text, const std::string& fontName, int fontSize)
	{
		TransformData transformData = TransformData(pos, scale, rot);

		TextData textData = TextData(fontName, text, fontSize);
		SpriteComponentData spriteData = SpriteComponentData(spr);
		SpriteComponentData highlightSpriteData = SpriteComponentData(highlightSpr);
		ButtonComponentData btnData = { spriteData, highlightSpriteData, textData };

		return createGameObject(transformData, ZERO_SPRITE_DATA, ZERO_LABEL_DATA, btnData);
	}



	void GameObjectManager::addAudioSource(int objId, const std::string& audio, float vol, float pan, bool enabled)
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

	void GameObjectManager::addBoxCollider(int objId, int w, int h, const std::string& t, bool enabled)
	{
		auto it = mGameObjMap.find(objId);
		if (it != mGameObjMap.end())
		{
			BoxColliderData data = { w,h,t };
			ComponentManager* pComponentManager = EngineCore::getInstance()->getComponentManager();
			ComponentId newBoxColliderId = pComponentManager->allocateBoxColliderComponent(data);
			it->second->connectCollider(newBoxColliderId);
		}
	}

	void GameObjectManager::addSprite(int objId, const std::string& spriteToLoad, const std::string& tileName, Color color, bool enabled)
	{
		auto it = mGameObjMap.find(objId);
		if (it != mGameObjMap.end())
		{
			SpriteComponentData spriteData = SpriteComponentData(spriteToLoad, tileName, color, enabled);
			ComponentManager* pComponentManager = EngineCore::getInstance()->getComponentManager();
			ComponentId newSpriteId = pComponentManager->allocateSpriteComponent(spriteData);
			it->second->connectSprite(newSpriteId);
		}
	}

	void GameObjectManager::addUILabel(int objId, const std::string& font, const std::string& text, int size, Color color, bool enabled)
	{
		auto it = mGameObjMap.find(objId);
		if (it != mGameObjMap.end())
		{
			TextData data = TextData(font, text, size, color, enabled);
			ComponentManager* pComponentManager = EngineCore::getInstance()->getComponentManager();
			ComponentId id = pComponentManager->allocateTextComponent(data);
			it->second->connectLabel(id);
		}
	}

	void GameObjectManager::addButton(int objId, const std::string& font, const std::string& text, int size, Color textColor, 
									  const std::string& spriteToLoad, const std::string& tileName, Color sprColor, const std::string& highlightSpriteToLoad, 
									  const std::string& highlightTileName, Color highlightSprColor, bool enabled)
	{
		auto it = mGameObjMap.find(objId);
		if (it != mGameObjMap.end())
		{
			TextData textData = TextData(font, text, size, textColor);
			SpriteComponentData spriteData = SpriteComponentData(spriteToLoad);
			SpriteComponentData highlightSpriteData = SpriteComponentData(highlightSpriteToLoad);
			ButtonComponentData btnData = ButtonComponentData(spriteData, highlightSpriteData, textData, enabled);
			
			ComponentManager* pComponentManager = EngineCore::getInstance()->getComponentManager();
			ComponentId id = pComponentManager->allocateButtonComponent(btnData);
			it->second->connectButton(id);
		}
	}

	void GameObjectManager::addNativeScript(std::shared_ptr<Actor> actor, int objId, bool enabled)
	{
		auto it = mGameObjMap.find(objId);
		if (it != mGameObjMap.end())
		{
			ComponentManager* pComponentManager = EngineCore::getInstance()->getComponentManager();
			ComponentId id = pComponentManager->allocateNativeScriptComponent(objId);
			it->second->connectNativeScript(id);

			if(actor != nullptr)
				it->second->getNativeScript()->setInstance(actor, objId, enabled);
		}
	}

	GameObject* GameObjectManager::createPlayer(const std::string& texture, glm::vec2 position, glm::vec2 scale, float rotation)
	{
		TransformData transformData = TransformData(position, scale, rotation);
		SpriteComponentData spriteData = SpriteComponentData(texture);

		return createGameObject(transformData, spriteData, ZERO_LABEL_DATA, ZERO_BTN_DATA, PLAYER_OBJ_ID);
	}


	GameObject* GameObjectManager::findGameObject(const GameObjectId& id) const
	{
		auto it = mGameObjMap.find(id);

		if (it != mGameObjMap.end())
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

			//remove components from manager
			ComponentManager* pComponentManager = EngineCore::getInstance()->getComponentManager();
			pComponentManager->deallocateNativeScriptComponent(obj->getNativeScriptId());
			pComponentManager->deallocateTransformComponent(obj->getTransformId());
			pComponentManager->deallocateSpriteComponent(obj->getSpriteId());
			pComponentManager->deallocateTextComponent(obj->getLabelId());
			pComponentManager->deallocateBoxColliderComponent(obj->getColliderId());
			pComponentManager->deallocateAudioSourceComponent(obj->getAudioSourceId());
			pComponentManager->deallocateButtonComponent(obj->getButtonId());
			
			//remove from map
			mGameObjMap.erase(it);
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

	void GameObjectManager::setButtonCallback(uint32 handle, ButtonCallbackVoid callback)
	{
		auto gameObj = findGameObject(handle);
		if (gameObj != nullptr)
		{
			auto btnComp = gameObj->getButton();
			if (btnComp != nullptr)
			{
				btnComp->setOnSelected(callback);
			}
		}
	}

	int GameObjectManager::getNumGameObjects()
	{
		return mGameObjMap.size();
	}

}