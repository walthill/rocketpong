#include "SceneManager.h"
#include "RocketEngine/asset/Serialization.h"
#include <RocketEngine/actor/Actor.h>
#include <RocketEngine/core/Log.h>
#include <RocketEngine/core/EngineCore.h>
#include <RocketEngine/gameobject/GameObjectManager.h>

namespace RKTEngine
{

	SceneManager::SceneManager()
	{
		mScenes = std::map<std::string, Scene*>();
	}

	SceneManager::~SceneManager()
	{
		cleanupScenes();
	}

	void SceneManager::cleanupScenes()
	{
		for (auto& s : mScenes)
		{
			cleanSceneElements(s.second);
			s.second->entities.clear();
		}
		mScenes.clear();
	}

	void SceneManager::cleanActiveScene()
	{
		if (mpActiveScene != nullptr)
		{
			auto sceneName = mpActiveScene->name;
			cleanSceneElements(mpActiveScene);
			mScenes.erase(sceneName);
			mpActiveScene = nullptr;
		}
	}

	void SceneManager::cleanSceneElements(Scene* scene)
	{
		for (auto& entity : scene->entities)
		{
			if (entity != nullptr)
			{
				RocketEngine->getEntityManager()->destroy(entity->getId());
				entity = nullptr;
			}
		}
		scene->entities.clear();
	}

	void SceneManager::registerEntity(GameObject* obj)
	{
		if (mpActiveScene != nullptr)
			mpActiveScene->entities.push_back(obj);
	}


	void SceneManager::beginScene(const std::string& sceneName)
	{
		//create the scene
		if (mScenes.find(sceneName) != mScenes.end())
		{
			mpActiveScene = mScenes[sceneName];
		}
		else
		{
			mpActiveScene = new Scene{ sceneName, std::vector<GameObject*>() };
			mScenes[sceneName] = mpActiveScene;
		}
	}

	void SceneManager::endScene(bool destroySceneData)
	{
		//Serialization::serializeScene(mpActiveScene);
		if(destroySceneData)	
			cleanActiveScene();
	}

	void SceneManager::loadScene(const std::string& sceneName, bool destroySceneData, bool makeActiveScene)
	{
		if (makeActiveScene)
		{
			if (destroySceneData)
				cleanActiveScene();

			const auto& sceneToLoad = mScenes.find(sceneName);
			if (sceneToLoad != mScenes.end())
			{
				mpActiveScene = sceneToLoad->second;
			}
			else
			{
				//mScenes[sceneName] = Serialization::deserializeScene(sceneName);
				mpActiveScene = mScenes[sceneName];
			}
		}
		else
		{
			//mScenes[sceneName] = Serialization::deserializeScene(sceneName);
		}

		//TODO: investigate deserializing increasing mem footprint 
		//RKT_MEMREPORT();
	}

	bool SceneManager::isActiveScene(const std::string& sceneName)
	{
		if (mScenes.find(sceneName) != mScenes.end())
		{
			return mScenes[sceneName]->name == mpActiveScene->name;
		}

		return false;
	}

	GameObject* SceneManager::findGameObjectInScene(uint32_t id)
	{
		const auto& objs = mpActiveScene->entities;
		for (size_t i = 0; i < objs.size(); i++)
		{
			if (objs[i]->getId() == id)
				return objs[i];
		}

		return nullptr;
	}

}