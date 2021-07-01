#include "EngineCore.h"
#include "RenderCore.h"
#include "Log.h"
#include "InputSystem.h"
#include "MessageManager.h"

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
		delete mpMasterTimer;
		delete mpMessageManager;
		delete mpInputSystem;
		delete mpRenderCore;
	}

	bool EngineCore::initialize()
	{
		mpMasterTimer = new RKTUtil::Timer();
		mDeltaTime = 0.0f;

		if (!initRenderCore())
		{
			return false;
		}
	
		initInputSystem();

		mpMasterTimer->start();
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
		mpMessageManager = new MessageManager();
	}

	void EngineCore::update()
	{
		mpMessageManager->processMessagesForThisFrame(mDeltaTime);
		mpInputSystem->processInput();
		calculateDeltaTime();
	}

	void EngineCore::render()
	{
		mpRenderCore->beginRender();
		mpRenderCore->render();
		mpRenderCore->endRender();
	}

	void EngineCore::onMessage(Message& message)
	{

	}

	void EngineCore::calculateDeltaTime()
	{
		float currentFrame = (float)getTime();
		mDeltaTime = currentFrame - mLastFrame;
		mLastFrame = currentFrame;
	}

	double EngineCore::getTime() { return mpMasterTimer->getTimeElapsedMs(); }
	InputSystem* EngineCore::getInputSystem() { return mpInputSystem; }
	MessageManager* EngineCore::getMessageManager() { return mpMessageManager; }

}