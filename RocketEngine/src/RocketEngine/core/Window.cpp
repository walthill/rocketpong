/********
	=========================
			 ROCKET ENGINE
	=========================
	File Created By: Walter Hill

	Rocket3D is an open source 3D game engine written using C++ & OpenGL.

	This class makes use of tutorials from Learn OpenGL
	(https://learnopengl.com)

	This code is open source under the Apache 2.0 license.
	(https://github.com/walthill/Rocket3D/blob/master/LICENSE)

	=========================
			 Window.cpp
	=========================

********/

#include "Window.h"
#include "Log.h"
#include <glad/glad.h>
#include <glfw3.h>

namespace RKTEngine
{
	bool Window::mShouldInitGLFW = true;

	Window::Window()
	{
		mpWindow = nullptr;
	}

	Window::~Window()
	{
		clean();
	}

	void Window::clean()
	{
		destroyGLFW();
	}

	bool Window::initialize(int width, int height, const char* windowName, int settingsFlags, bool showCursor)
	{
		initGLFW();

		//Init window
		mpWindow = glfwCreateWindow(width, height, windowName, nullptr, nullptr);

		if (mpWindow == nullptr)
		{
			RKT_CORE_ERROR("ERROR::WINDOW::FAILED to create GLFW Window");
			glfwTerminate();
			return false;
		}

		setWindowToCurrent();

		//Init GLAD fuction pointer handling - requires a window for some reason?
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			RKT_CORE_ERROR("ERROR::WINDOW::FAILED to initialize GLAD");
			glfwTerminate();
			return false;
		}

		setViewport(TOP_LEFT, TOP_LEFT, width, height);
		enableWindowFlags(settingsFlags);
		setCursor(showCursor);

		return true;
	}

	void Window::initGLFW()
	{
		if (mShouldInitGLFW)
		{
			glfwInit();

			//Init OpenGL version settings
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_SAMPLES, 4);

			mShouldInitGLFW = false;
		}
		else
		{
			RKT_CORE_WARN("Attemping to reinitialize GLFW");
		}
	}


	void Window::enableWindowFlags(int settingsToEnable)
	{
		if (settingsToEnable & AA_MULTISAMPLE)
			glEnable(GL_MULTISAMPLE);
		if (settingsToEnable & DEPTH_TEST)
			glEnable(GL_DEPTH_TEST);
		if (settingsToEnable & STENCIL_TEST)
		{
			glEnable(GL_STENCIL_TEST);

			/*
				//How to update the stencil buffer when a fragment passes the given test

				glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)

				sfail: action to take if the stencil test fails.
				dpfail: action to take if the stencil test passes, but the depth test fails.
				dppass: action to take if both the stencil and the depth test pass.
			*/
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		}
		if (settingsToEnable & CULL_FACE)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		if (settingsToEnable & BLEND)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			//glBlendFunc(GL_ONE, GL_ZERO);
		}
		if (settingsToEnable & GAMMA_CORRECT)
		{
			glEnable(GL_FRAMEBUFFER_SRGB);
		}
	}

	void Window::disableWindowFlags(int settingsToDisable)
	{
		if (settingsToDisable & AA_MULTISAMPLE)
			glDisable(GL_MULTISAMPLE);
		if (settingsToDisable & DEPTH_TEST)
			glDisable(GL_DEPTH_TEST);
		if (settingsToDisable & STENCIL_TEST)
			glDisable(GL_STENCIL_TEST);
		if (settingsToDisable & CULL_FACE)
			glDisable(GL_CULL_FACE);
		if (settingsToDisable & BLEND)
			glDisable(GL_BLEND);
		if (settingsToDisable & GAMMA_CORRECT)
			glDisable(GL_FRAMEBUFFER_SRGB);
	}

	void Window::setViewport(int x, int y)
	{
		glViewport(x, y, mWidth, mHeight);
	}

	void Window::setViewport(int x, int y, int width, int height)
	{
		setScreenDimensions(width, height);
		glViewport(x, y, mWidth, mHeight);
	}

	void Window::swapBuffers()
	{
		// swap the buffers
		glfwSwapBuffers(mpWindow);
	}

	void Window::setWindowDrawMode(WindowDrawFace faceToDraw, WindowDrawMode drawMode)
	{
		if (faceToDraw == FRONT)
		{
			if (drawMode == WIREFRAME)
				glPolygonMode(GL_FRONT, GL_LINE);
			else if (drawMode == FILL)
				glPolygonMode(GL_FRONT, GL_FILL);
			else if (drawMode == VERTICES)
				glPolygonMode(GL_FRONT, GL_POINT);
		}
		else if (faceToDraw == BACK)
		{
			if (drawMode == WIREFRAME)
				glPolygonMode(GL_BACK, GL_LINE);
			else if (drawMode == FILL)
				glPolygonMode(GL_BACK, GL_FILL);
			else if (drawMode == VERTICES)
				glPolygonMode(GL_BACK, GL_POINT);
		}
		else if (faceToDraw == FRONT_AND_BACK)
		{
			if (drawMode == WIREFRAME)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else if (drawMode == FILL)
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			else if (drawMode == VERTICES)
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		}
	}

	void Window::setCursor(bool shouldAppear)
	{
		if (shouldAppear)
			glfwSetInputMode(mpWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(mpWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void Window::toggleWireframe(bool showWireframe)
	{
		if (showWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void Window::setWindowToCurrent()
	{
		glfwMakeContextCurrent(mpWindow);
	}

	GLFWwindow* Window::getWindowHandle() const
	{
		return mpWindow;
	}

	void Window::setScreenDimensions(int width, int height)
	{
		mWidth = width;
		mHeight = height;
	}

	void Window::setHeight(int height)
	{
		mHeight = height;
	}

	void Window::setWidth(int width)
	{
		mWidth = width;
	}

	void Window::destroyGLFW()
	{
		glfwTerminate();
	}
}