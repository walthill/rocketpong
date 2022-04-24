#ifndef RENDER_CORE_H
#define RENDER_CORE_H

namespace RKTEngine
{
	class Window;
	class ShaderManager;
	class ComponentManager;
	class Texture2D;
	
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
			void logRenderStats();

			inline Window* getWindow() { return mpWindowHandle; }
			inline ShaderManager* getShaderManager() { return mpShaderManager; }
			
		private:
			Window* mpWindowHandle;
			ShaderManager* mpShaderManager;
			Texture2D* tex;

			bool createWindow();
			void init2DShaderData();
	};
}

#endif