#ifndef SCENE_MAN_H
#define SCENE_MAN_H

#include <RKTUtils/Trackable.h>
#include <map>
#include <vector>
#include <cereal/types/vector.hpp>
#include <RocketEngine/Defines.h>
#include <RocketEngine/gameobject/GameObject.h>
#include <RocketEngine/core/EngineCore.h>
#include <RocketEngine/gameobject/GameObjectManager.h>

namespace RKTEngine
{
	struct Scene
	{
		std::string name;
		std::vector<GameObject*> entities;

		template<class Archive>
		void save(Archive& archive) const
		{
			std::vector<GameObject> entitiesToSerialize = std::vector<GameObject>();
			for (size_t i = 0; i < entities.size(); i++)
			{
				entitiesToSerialize.push_back(*entities[i]);
			}

			{
				archive(MAKE_NVP("GameObjects", entitiesToSerialize));
			}
		}

		template<class Archive>
		void load(Archive& archive)
		{
			std::vector<GameObject> entitiesToDeserialize = std::vector<GameObject>();
			{
				archive(MAKE_NVP("GameObjects", entitiesToDeserialize));
			}

			for (size_t i = 0; i < entitiesToDeserialize.size(); i++)
			{
				auto newObj = EngineCore::getInstance()->getEntityManager()->registerGameObjectData(entitiesToDeserialize[i]);
				entities.push_back(newObj);
			}
		}
	};

	class SceneManager : public RKTUtil::Trackable
	{
	public:
		SceneManager();
		~SceneManager();

		void cleanActiveScene();

		void registerEntity(GameObject* obj);

		/// <summary>
		/// Mark beginning of scene data. Actors between this call and endScene() will be assigned to sceneName
		/// </summary>
		/// <param name="sceneName">: Name of scene to generate</param>
		void beginScene(const std::string& sceneName);

		/// <summary>
		/// End scene data collection
		/// </summary>
		/// <param name="destroySceneData">: Choose to keep scene data in memory</param>
		void endScene(bool destroySceneData = true);

		void loadScene(const std::string& sceneName, bool destroySceneData = true, bool makeActiveScene = true);
		bool isActiveScene(const std::string& sceneName);

		GameObject* findGameObjectInScene(uint32_t id);

	private:
		std::map<std::string, Scene*> mScenes;
		Scene* mpActiveScene = nullptr;

		void cleanupScenes();
		void cleanSceneElements(Scene* scene);
	};
}

#endif // !SCENE_MAN_H
