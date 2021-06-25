#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

namespace RKTEngine
{
	class RenderCore;
	class InputSystem;

	class EngineCore
	{
		public:
			EngineCore();
			~EngineCore();

			void clean();
			bool initialize();
			
			void update();
			void render();

		private:
			RenderCore* mpRenderCore = nullptr;
			InputSystem* mpInputSystem = nullptr;

			bool initRenderCore();
			void initInputSystem();
	};
}

#endif // !ENGINE_CORE_H
