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
#include "Window.h" 
#include "RocketEngine/input/WindowCallbacks.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/core/MessageManager.h"
#include "RocketEngine/input/MessageDefines.h"

namespace RKTEngine
{
	WindowCallbacks* InputSystem::spCallbacks = nullptr;
	MessageManager* InputSystem::spMessageManagerHandle = nullptr;
	
	InputSystem::InputSystem(Window* window)
	{
		spCallbacks = new WindowCallbacks();
		spCallbacks->initialize(window);
		spMessageManagerHandle = EngineCore::getInstance()->getMessageManager();
	}

	InputSystem::~InputSystem()
	{
		delete spCallbacks;
		spMessageManagerHandle = nullptr; //InputSystem does NOT own this
	}

#pragma region Callback Handling

	void InputSystem::onWindowClose()
	{
		Message* pMessage = new ExitMessage();
		spMessageManagerHandle->addMessage(pMessage, 1);
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

	void InputSystem::onKeyDownEvent(int button, int scancode, int modifier, int repeat)
	{
		Message* pMessage = new KeyDownMessage(button, repeat);
		spMessageManagerHandle->addMessage(pMessage, 1);
	}
	
	void InputSystem::onKeyUpEvent(int button, int scancode, int modifier)
	{
		Message* pMessage = new KeyUpMessage(button);
		spMessageManagerHandle->addMessage(pMessage, 1);
	}

#pragma endregion

#pragma region Polling

	bool InputSystem::getKeyDown(const KeyCode& key)
	{
		return spCallbacks->getKeyDown(key);
	}
	bool InputSystem::getKeyUp(const KeyCode& key)
	{
		return spCallbacks->getKeyUp(key);
	}

#pragma endregion


	void InputSystem::processInput()
	{
		Message* pMessage = new UpdateMessage();
		spMessageManagerHandle->addMessage(pMessage, 1);

		spCallbacks->pollEvents();
	}
}