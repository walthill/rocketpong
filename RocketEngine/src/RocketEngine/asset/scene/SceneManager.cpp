#include "SceneManager.h"
#include "RocketEngine/asset/Serialization.h"
#include <RocketEngine/actor/UILabel.h>
#include <RocketEngine/actor/Actor.h>
#include <RocketEngine/core/Log.h>

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
			cleanUIElements(s.second);
			s.second->entities.clear();
		}
		mScenes.clear();
	}

	void SceneManager::cleanActiveScene()
	{
		if (mpActiveScene != nullptr)
		{
			auto sceneName = mpActiveScene->name;
			cleanUIElements(mpActiveScene);
			mScenes.erase(sceneName);
			mpActiveScene = nullptr;
		}
	}

	void SceneManager::cleanUIElements(Scene* scene)
	{
		for (auto& text : scene->textUIs)
		{
			if (text != nullptr)
			{
				delete text;
				text = nullptr;
			}
		}
		scene->textUIs.clear();
	}


	void SceneManager::registerUI(UILabel* textUI)
	{
		if(mpActiveScene != nullptr)
			mpActiveScene->textUIs.push_back(textUI);
	}

	void SceneManager::registerEntity(Actor* actor)
	{
		//mpActiveScene->entities.push_back(actor);
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
			mpActiveScene = new Scene{ sceneName, std::vector<UILabel*>() };
			mScenes[sceneName] = mpActiveScene;
		}
	}

	void SceneManager::endScene(bool destroySceneData)
	{
		Serialization::serializeScene(mpActiveScene);
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
				mScenes[sceneName] = Serialization::deserializeScene(sceneName);
				mpActiveScene = mScenes[sceneName];
			}
		}
		else
		{
			mScenes[sceneName] = Serialization::deserializeScene(sceneName);
		}

		RKT_MEMREPORT();
	}

	bool SceneManager::isActiveScene(const std::string& sceneName)
	{
		if (mScenes.find(sceneName) != mScenes.end())
		{
			return mScenes[sceneName]->name == mpActiveScene->name;
		}

		return false;
	}

	UILabel* SceneManager::findUILabelInScene(const std::string& objName)
	{
		const auto& labels = mpActiveScene->textUIs;
		for (size_t i = 0; i < labels.size(); i++)
		{
			if (strcmp(labels[i]->mpGameObject->name.c_str(), objName.c_str()) == 0)
			{
				return labels[i];
			}
		}

		return nullptr;
	}

}