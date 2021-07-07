#include "RenderCore.h"
#include "Window.h"
#include "RocketEngine/render/shader/ShaderManager.h"

namespace RKTEngine
{
	using BufferType = Renderer::BufferType;

	RenderCore::RenderCore()
	{
		mpWindowHandle = nullptr;
		mpShaderManager = nullptr;
	}

	RenderCore::~RenderCore()
	{
		clean();
	}

	void RenderCore::clean()
	{
		delete mpShaderManager;
		delete mpWindowHandle;
	}


	bool RenderCore::initialize()
	{
		if (!createWindow())
		{
			return false;
		}

		mpShaderManager = new ShaderManager();

		return true;
	}

	void RenderCore::beginRender()
	{
		RenderCommand::clearColor(Color::blue);
		RenderCommand::clearBuffer(BufferType::COLOR_BUFFER);
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