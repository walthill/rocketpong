#ifndef RENDER_CORE_H
#define RENDER_CORE_H

#include <RocketEngine/render/Renderer.h>

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
			void render(ComponentManager* componentsToDraw, float deltaTime);
			void endRender();

			inline Window* getWindow() { return mpWindowHandle; }
			inline ShaderManager* getShaderManager() { return mpShaderManager; }
			inline std::shared_ptr<VertexArray> getSpriteVertexData() { return mSpriteVA; }
			inline std::shared_ptr<VertexBuffer> getSpriteBufferData() { return mSpriteVB; }
			inline static Renderer::API getAPI() { return Renderer::getAPI(); }

			static void submit(const std::shared_ptr<VertexArray>& vertexArray, int instanceCount = 1);
		private:
			std::shared_ptr<VertexArray> mSpriteVA;
			std::shared_ptr<VertexBuffer> mSpriteVB;

			Window* mpWindowHandle;
			ShaderManager* mpShaderManager;
			Texture2D* tex;

			bool createWindow();
			void init2DVertexData();
			void init2DShaderData();
	};
}

#endif