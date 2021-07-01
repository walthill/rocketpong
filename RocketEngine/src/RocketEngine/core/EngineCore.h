#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#include <RKTUtils/Singleton.h>
#include <RKTUtils/Timer.h>
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

			void calculateDeltaTime();

			double getTime();
			inline float getDeltaTime() { return mDeltaTime; }

			InputSystem* getInputSystem();		
			MessageManager* getMessageManager();

		private:
			RKTUtil::Timer* mpMasterTimer = nullptr;

			RenderCore* mpRenderCore = nullptr;
			InputSystem* mpInputSystem = nullptr;
			MessageManager* mpMessageManager = nullptr;

			float mDeltaTime = 0 , mLastFrame =0 ;

			bool initRenderCore();
			void initInputSystem();
	};
}

#endif // !ENGINE_CORE_H
