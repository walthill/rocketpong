#include "SceneManager.h"
#include "RocketEngine/asset/Serialization.h"
#include <RocketEngine/core/Log.h>
#include "RocketEngine/input/KeyCodes.h"

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
				EngineCore::getInstance()->getEntityManager()->destroy(entity->getId());
				entity = nullptr;
			}
		}
		scene->entities.clear();
	}

	void SceneManager::registerEntity(GameObject* obj)
	{
		if (mpActiveScene != nullptr)
		{
			mpActiveScene->entities.push_back(obj);
			
			auto buttonComp = obj->getButton();
			if (buttonComp != nullptr)
			{
				mpActiveScene->uiManager.buttons.push_back(buttonComp);
			}
		}
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
		Serialization::serializeScene(mpActiveScene);
		if (destroySceneData)
		{
			cleanActiveScene();
		}
	}

	void SceneManager::loadScene(const std::string& sceneName, bool destroySceneData, bool makeActiveScene)
	{
		RKT_PROFILE_FUNCTION();

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
				mpActiveScene = new Scene{ sceneName, std::vector<GameObject*>() };
				mpActiveScene = Serialization::deserializeScene(sceneName);
				mScenes[sceneName] = mpActiveScene;

				if(mpActiveScene->uiManager.buttons.size() > 0)
					mpActiveScene->uiManager.buttons[0]->setHighlighted(true);
			}
		}
		else
		{
			mScenes[sceneName] = Serialization::deserializeScene(sceneName);
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

	void SceneManager::onMessage(RKTEngine::Message& message)
	{
		RKTEngine::MessageDispatcher dispatcher(message);
		dispatcher.dispatch<RKTEngine::KeyDownMessage>(RKT_BIND_MESSAGE_FN(SceneManager::onKeyDown));
	}

	bool SceneManager::onKeyDown(RKTEngine::KeyDownMessage& msg)
	{
		const auto& keyCode = msg.getKeyCode();
		if (keyCode == Key::S || keyCode == Key::Down || keyCode == Key::D || keyCode == Key::Right)
			updateButtonNavigation(true);		
		else if (keyCode == Key::W || keyCode == Key::Up || keyCode == Key::A || keyCode == Key::Left)
			updateButtonNavigation(false);
		else if (keyCode == Key::Enter || keyCode == Key::Space || keyCode == Key::KPEnter)
		{
			if (mpActiveScene != nullptr)
			{
				const auto& uiManager = mpActiveScene->uiManager;
				if (!uiManager.buttons.empty())
				{
					uiManager.buttons[uiManager.currentButtonIndex]->onSelected();
				}
			}
		}

		return true;
	}

	void SceneManager::updateButtonNavigation(bool moveDown)
	{
		RKT_PROFILE_FUNCTION();

		if (mpActiveScene == nullptr)
			return;

		auto& uiManager = mpActiveScene->uiManager;
		if (uiManager.buttons.empty())
			return;

		if (moveDown)
		{			
			if (uiManager.buttons[uiManager.currentButtonIndex]->mIsEnabled)
			{
				uiManager.buttons[uiManager.currentButtonIndex]->setHighlighted(false);
				if (uiManager.currentButtonIndex == uiManager.buttons.size() - 1)
				{
					uiManager.currentButtonIndex = 0;
				}
				else
				{
					uiManager.currentButtonIndex += 1;
				}

				uiManager.buttons[uiManager.currentButtonIndex]->setHighlighted(true);
			}			
		}
		else
		{
			if (uiManager.buttons[uiManager.currentButtonIndex]->mIsEnabled)
			{
				uiManager.buttons[uiManager.currentButtonIndex]->setHighlighted(false);
				if (uiManager.currentButtonIndex == 0)
				{
					uiManager.currentButtonIndex = uiManager.buttons.size() - 1;
				}
				else
				{
					uiManager.currentButtonIndex += -1;
				}

				uiManager.buttons[uiManager.currentButtonIndex]->setHighlighted(true);
			}
		}
	}

	GameObject* SceneManager::findGameObjectInScene(uint32_t id)
	{
		RKT_PROFILE_FUNCTION();

		const auto& objs = mpActiveScene->entities;
		for (size_t i = 0; i < objs.size(); i++)
		{
			if (objs[i]->getId() == id)
				return objs[i];
		}

		return nullptr;
	}

}