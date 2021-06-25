#include "EngineCore.h"
#include "RenderCore.h"
#include "../input/InputSystem.h"
#include "Log.h"

namespace RKTEngine
{
	EngineCore::EngineCore()
	{

	}

	EngineCore::~EngineCore()
	{
		clean();
	}

	void EngineCore::clean()
	{
		delete mpRenderCore;
		delete mpInputSystem;
	}

	bool EngineCore::initialize()
	{
		if (!initRenderCore())
		{
			return false;
		}
	
		initInputSystem();

		return true;
	}

	bool EngineCore::initRenderCore()
	{
		mpRenderCore = new RenderCore();
		if (!mpRenderCore->initialize())
		{
			return false;
		}

		return true;
	}

	void EngineCore::initInputSystem()
	{
		mpInputSystem = new InputSystem(mpRenderCore->getWindow());
	}

	void EngineCore::update()
	{
		mpInputSystem->processInput();
	}

	void EngineCore::render()
	{
		mpRenderCore->beginRender();
		mpRenderCore->render();
		mpRenderCore->endRender();
	}
}