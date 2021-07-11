#ifndef RENDER_CORE_H
#define RENDER_CORE_H

#include "RocketEngine/render/RenderCommand.h"

namespace RKTEngine
{
	class Window;
	class ShaderManager;
	class ComponentManager;

	class RenderCore
	{
		public:
			RenderCore();
			~RenderCore();

			bool initialize();
			void clean();

			void beginRender();
			void render(ComponentManager* componentsToDraw);
			void endRender();

			inline Window* getWindow() { return mpWindowHandle; }
			inline ShaderManager* getShaderManager() { return mpShaderManager; }
			inline static std::shared_ptr<VertexArray> getSpriteVertexData() { return mSpriteVA; }
			inline static Renderer::API getAPI() { return Renderer::getAPI(); }

		private:
			static std::shared_ptr<VertexArray> mSpriteVA;
			
			Window* mpWindowHandle;
			ShaderManager* mpShaderManager;

			bool createWindow();
			void init2DVertexData();
			void init2DShaderData();
	};
}

#endif