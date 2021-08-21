/********
	=========================
			 ROCKET ENGINE
	=========================
	File Created By: Walter Hill

	Rocket3D is an open source 3D game engine written using C++ & OpenGL.

	This code is open source under the Apache 2.0 license.
	(https://github.com/walthill/Rocket3D/blob/master/LICENSE)

	=========================
			 InputSystem.h
	=========================
	This class handles user input and converts those inputs 
	into messages with correspoding actions set using the GameMessage base class
	
********/

#ifndef INPUT_SYS_H
#define INPUT_SYS_H

#include <RKTUtils/Trackable.h>
#include "RocketEngine/input/KeyCodes.h"
#include "RocketEngine/input/MouseCodes.h"

namespace RKTEngine
{
	class Window;
	class WindowCallbacks;
	class MessageManager;

	class InputSystem : public RKTUtil::Trackable
	{
		public:
			
			/***
				* Constructor that takes a GLFW Window context
			***/
			InputSystem(Window* window);

			~InputSystem();


		#pragma region Callback Input
			/***
				* Callback for resizing the window
			***/
			void onMouseScroll(double xoffset, double yoffset);

			/***
				* Callback for mouse clicks
			***/
			void onMouseClick(int button, int action, int modifier, double xpos, double ypos);

			/***
				* Callback for mouse movement
			***/
			void onMouseMove(double xpos, double ypos);

			void onKeyDownEvent(int button, int scancode, int modifier, int repeat);
			void onKeyUpEvent(int button, int scancode, int modifier);

			void onWindowClose();

	#pragma endregion


	#pragma region Polling Input
			static bool getKeyDown(const KeyCode& button);
			static bool getKeyUp(const KeyCode& button);
			static bool getMouseButtonDown(const MouseCode& button);
			static bool getMouseButtonUp(const MouseCode& button);
	#pragma endregion

			/***
				* Checks for inputs every frame and queues input message
			***/
			void processInput();

		private:
			static WindowCallbacks* spCallbacks;
			static MessageManager* spMessageManagerHandle;
	};
}

#endif // !INPUT_SYS_H
