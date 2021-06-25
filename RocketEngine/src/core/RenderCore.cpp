#include "RenderCore.h"
#include "Window.h"

namespace RKTEngine
{
	RenderCore::RenderCore()
	{
		mpWindowHandle = nullptr;
	}

	RenderCore::~RenderCore()
	{
		clean();
	}

	void RenderCore::clean()
	{
		delete mpWindowHandle;
	}


	bool RenderCore::initialize()
	{
		if (!createWindow())
		{
			return false;
		}

		return true;
	}

	void RenderCore::beginRender()
	{
	}

	void RenderCore::render()
	{

	}

	void RenderCore::endRender()
	{
		mpWindowHandle->swapBuffers();
	}

	bool RenderCore::createWindow()
	{
		mpWindowHandle = new Window();
		if (!mpWindowHandle->initialize(1280, 720, "RogueOne"))
		{
			return false;
		}

		return true;
	}
}