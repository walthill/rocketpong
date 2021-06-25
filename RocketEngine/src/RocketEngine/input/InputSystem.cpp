/********
	=========================
			 ROCKET ENGINE
	=========================
	File Created By: Walter Hill

	Rocket3D is an open source 3D game engine written using C++ & OpenGL.

	This code is open source under the Apache 2.0 license.
	(https://github.com/walthill/Rocket3D/blob/master/LICENSE)

	=========================
			 InputSystem.cpp
	=========================

********/

#include "InputSystem.h"
#include "../core/Window.h" 
#include "WindowCallbacks.h"

namespace RKTEngine
{
	InputSystem::InputSystem(Window* window)
	{
		mpCallbacks = new WindowCallbacks();
		mpCallbacks->initialize(window);
	}

	InputSystem::~InputSystem()
	{
		delete mpCallbacks;
	}

	void InputSystem::onMouseScroll(double xoffset, double yoffset)
	{
	}

	void InputSystem::onMouseClick(int button, int action, int modifier, double xpos, double ypos)
	{
	}

	void InputSystem::onMouseMove(double xpos, double ypos)
	{
	}


	void InputSystem::onKeyEvent(int key, int scancode, int action, int mods)
	{
	}

	void InputSystem::processInput()
	{
		mpCallbacks->pollEvents();
	}
}