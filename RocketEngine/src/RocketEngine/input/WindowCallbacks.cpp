#include "WindowCallbacks.h"
#include "InputSystem.h"
#include "../core/Window.h"
#include <glfw3.h>

namespace RKTEngine
{
	#pragma region Callbacks

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
	void mouse_click_callback(GLFWwindow* window, int button, int action, int modifier);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		WindowCallbacks* wind = reinterpret_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window));
		wind->onWindowResize(width, height);
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		InputSystem* wind = reinterpret_cast<InputSystem*>(glfwGetWindowUserPointer(window));
		wind->onKeyEvent(key, scancode, action, mods);
	}

	void mouse_click_callback(GLFWwindow* window, int button, int action, int modifier)
	{
		InputSystem* wind = reinterpret_cast<InputSystem*>(glfwGetWindowUserPointer(window));
		double xpos, ypos;

		//getting cursor position
		glfwGetCursorPos(window, &xpos, &ypos);
		wind->onMouseClick(button, action, modifier, xpos, ypos);
	}

	//Help found here https://stackoverflow.com/questions/27387040/referencing-glfws-callback-functions-from-a-class
	void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
	{
		InputSystem* wind = reinterpret_cast<InputSystem*>(glfwGetWindowUserPointer(window));
		wind->onMouseMove(xpos, ypos);
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		InputSystem* wind = reinterpret_cast<InputSystem*>(glfwGetWindowUserPointer(window));
		wind->onMouseScroll(xoffset, yoffset);
	}

	#pragma endregion

	void WindowCallbacks::initialize(Window* window)
	{
		mpWindowHandle = window;

		//give this class "user" access to callbacks
		glfwSetWindowUserPointer(window->getWindowHandle(), reinterpret_cast<void*>(this));

		auto wind = window->getWindowHandle();
		glfwSetCursorPosCallback(wind, mouse_move_callback);
		glfwSetScrollCallback(wind, scroll_callback);
		glfwSetMouseButtonCallback(wind, mouse_click_callback);
		glfwSetKeyCallback(wind, key_callback);
		glfwSetFramebufferSizeCallback(wind, framebuffer_size_callback);
	}

	void WindowCallbacks::onWindowResize(int width, int height)
	{
		mpWindowHandle->setViewport(0, 0, width, height);
	}

	void WindowCallbacks::pollEvents()
	{
		glfwPollEvents();
	}
}