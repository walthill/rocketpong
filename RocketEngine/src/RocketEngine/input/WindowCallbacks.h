#ifndef WINDOW_CALLBACK_H
#define WINDOW_CALLBACK_H

namespace RKTEngine
{
	class Window;

	class WindowCallbacks
	{
		public:
			void onWindowResize(int width, int height);
			void initialize(Window* wind);
			void pollEvents();

		private:
			Window* mpWindowHandle;
	};
}

#endif // !WINDOW_CALLBACK_H
