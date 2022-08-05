#ifndef SCENE_MAN_H
#define SCENE_MAN_H

#include <RKTUtils/Trackable.h>
#include <map>
#include <vector>

namespace RKTEngine
{
	class GameObject;
	
	struct Scene
	{
		std::string name;
		std::vector<GameObject*> entities;
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
