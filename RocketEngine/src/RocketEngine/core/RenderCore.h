#ifndef RENDER_CORE_H
#define RENDER_CORE_H

#include "RocketEngine/render/RenderCommand.h"

namespace RKTEngine
{
	class Window;
	class ShaderManager;

	class RenderCore
	{
		public:
			RenderCore();
			~RenderCore();

			bool initialize();
			void clean();

			void beginRender();
			void render();
			void endRender();

			inline Window* getWindow() { return mpWindowHandle; }
			inline static Renderer::API getAPI() { return Renderer::getAPI(); }

		private:
			Window* mpWindowHandle;
			ShaderManager* mpShaderManager;

			bool createWindow();
	};
}

#endif