#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#include <RKTUtils/Singleton.h>
#include <RKTUtils/Timer.h>
#include <RKTUtils/Profiling.h>
#include "ComponentManager.h"
#include "RocketEngine/gameobject/GameObjectManager.h"
#include "RocketEngine/asset/AssetManager.h"
#include "RocketEngine/input/message/Message.h"

namespace RKTEngine
{
	class RenderCore;
	class InputSystem;
	class MessageManager;
	
	class EngineCore : public RKTUtil::Singleton<EngineCore>
	{
		public:
			EngineCore();
			~EngineCore();

			void clean();
			bool initialize();
			
			void update();
			void render();

			void onMessage(Message& message);

			double getTime();

			RenderCore* getRenderer(); 
			inline ComponentManager* getComponentManager() { return mpComponentManager; }
			inline AssetManager* getAssetManager() { return mpAssetManager; }
			inline GameObjectManager* getEntityManager() { return mpEntityManager; }
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

			float mLastFrame = 0;

			bool initRenderCore();
			void initInputSystem();
			void calculateDeltaTime();
	};
}

#endif // !ENGINE_CORE_H
