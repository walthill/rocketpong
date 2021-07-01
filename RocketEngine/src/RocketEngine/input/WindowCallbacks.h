#ifndef WINDOW_CALLBACK_H
#define WINDOW_CALLBACK_H

#include "RocketEngine/core/InputSystem.h"

struct GLFWwindow;

namespace RKTEngine
{
	class Window;

	class WindowCallbacks
	{
		public:
			
			void onWindowResize(int width, int height);
			void initialize(Window* wind);
			void pollEvents();

	#pragma region Polling Input
			bool getKeyDown(const KeyCode& button);
			bool getKeyUp(const KeyCode& button);
			bool getMouseButtonDown(const MouseCode& button);
			bool getMouseButtonUp(const MouseCode& button);
	#pragma endregion

			inline Window* getWindowRef() { return mpWindow; }

		private:
			Window* mpWindow;

			void registerCallbacks(GLFWwindow* window);
	};
}

#endif // !WINDOW_CALLBACK_H
