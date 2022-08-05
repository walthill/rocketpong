#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#include <RKTUtils/Singleton.h>
#include <RKTUtils/Timer.h>
#include <RKTUtils/Profiling.h>
#include "RocketEngine/input/message/Message.h"

namespace RKTEngine
{
	class RenderCore;
	class InputSystem;
	class AudioManager;
	class MessageManager;
	class SceneManager;
	class ComponentManager;
	class AssetManager;
	class GameObjectManager;
	class ShaderManager;
	
	class EngineCore : public RKTUtil::Singleton<EngineCore>
	{
		friend class GameApp;

		public:
			EngineCore();
			~EngineCore();

			double getTime();

			ComponentManager* getComponentManager(); 
			AssetManager* getAssetManager(); 
			GameObjectManager* getEntityManager(); 
			SceneManager* getSceneManager();
			InputSystem* getInputSystem();
			MessageManager* getMessageManager();
			ShaderManager* getShaderManager();

			void showDebugWireframes(bool show);

			int getWindowHeight();
			int getWindowWidth();

			static float sDeltaTime;

		private:
			RKTUtil::Timer* mpMasterTimer = nullptr;

			RenderCore* mpRenderCore = nullptr;
			InputSystem* mpInputSystem = nullptr;
			MessageManager* mpMessageManager = nullptr;
			AssetManager* mpAssetManager = nullptr;
			GameObjectManager* mpEntityManager = nullptr;
			ComponentManager* mpComponentManager = nullptr;
			SceneManager* mpSceneManager = nullptr;
			AudioManager* mpAudioManager = nullptr;

			float mLastFrame = 0;

			bool initRenderCore();
			void initInputSystem();
			void calculateDeltaTime();
			void clean();
			bool initialize();
			void update();
			void render();
			void onMessage(Message& message);
			RenderCore* getRenderer();
	};
}

#endif // !ENGINE_CORE_H
