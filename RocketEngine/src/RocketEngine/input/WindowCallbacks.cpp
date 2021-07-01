#include "WindowCallbacks.h"
#include "RocketEngine/core/Window.h"
#include <glfw3.h>

namespace RKTEngine
{
#pragma region Callbacks

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
	void mouse_click_callback(GLFWwindow* window, int button, int action, int modifier);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void window_close_callback(GLFWwindow* window);

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

	void window_close_callback(GLFWwindow* window)
	{
		InputSystem* wind = reinterpret_cast<InputSystem*>(glfwGetWindowUserPointer(window));
		wind->onWindowClose();
	}

	#pragma endregion

	void WindowCallbacks::initialize(Window* window)
	{
		mpWindow = window;
		auto wind = window->getWindowHandle();

		//give this class "user" access to callbacks
		glfwSetWindowUserPointer(wind, reinterpret_cast<void*>(this));
		registerCallbacks(wind);
	}

	void WindowCallbacks::registerCallbacks(GLFWwindow* window)
	{
		glfwSetCursorPosCallback(window, mouse_move_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetMouseButtonCallback(window, mouse_click_callback);
		glfwSetKeyCallback(window, key_callback);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetWindowCloseCallback(window, window_close_callback);
	}

#pragma region GLFW Polling

	//polling
	bool WindowCallbacks::getKeyDown(const KeyCode& key)
	{
		auto state = glfwGetKey(mpWindow->getWindowHandle(), key);
		return state == GLFW_PRESS;
	}

	bool WindowCallbacks::getKeyUp(const KeyCode& key)
	{
		auto state = glfwGetKey(mpWindow->getWindowHandle(), key);
		return state == GLFW_RELEASE;
	}

#pragma endregion

	void WindowCallbacks::onWindowResize(int width, int height)
	{
		mpWindow->setViewport(0, 0, width, height);
	}

	void WindowCallbacks::pollEvents()
	{
		glfwPollEvents();
	}

}