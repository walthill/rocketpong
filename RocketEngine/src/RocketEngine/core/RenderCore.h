#ifndef RENDER_CORE_H
#define RENDER_CORE_H

namespace RKTEngine
{
	class Window;

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

		private:
			Window* mpWindowHandle;
			bool createWindow();
	};
}

#endif