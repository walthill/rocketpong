#include "EngineCore.h"
#include "RenderCore.h"
#include "Window.h"
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
		delete mpEntityManager;
		delete mpAssetManager;
		delete mpComponentManager;
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
		

		mpComponentManager = new ComponentManager(5000);
		mpEntityManager = new GameObjectManager(5000);

		mpAssetManager = new AssetManager();
		if (!mpAssetManager->initialize())
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
		mpMessageManager = new MessageManager();
		mpInputSystem = new InputSystem(mpRenderCore->getWindow());
	}

	void EngineCore::update()
	{
		mpMessageManager->processMessagesForThisFrame(mDeltaTime);

		mpComponentManager->update(mDeltaTime);
		mpEntityManager->updateAll(mDeltaTime);

		mpInputSystem->processInput();
		calculateDeltaTime();
	}

	void EngineCore::render()
	{
		mpRenderCore->beginRender();
		mpRenderCore->render(mpComponentManager);
		mpRenderCore->endRender();
	}

	void EngineCore::onMessage(Message& message)
	{
		//TODO: update orthographic projection on window resize
		mpEntityManager->onMessage(message);
	}

	void EngineCore::calculateDeltaTime()
	{
		float currentFrame = (float)getTime();
		mDeltaTime = currentFrame - mLastFrame;
		mLastFrame = currentFrame;
	}

	double EngineCore::getTime() { return mpMasterTimer->getTimeElapsedMs(); }
	RenderCore* EngineCore::getRenderer() { return mpRenderCore; }
	InputSystem* EngineCore::getInputSystem() { return mpInputSystem; }
	MessageManager* EngineCore::getMessageManager() { return mpMessageManager; }

	ShaderManager* EngineCore::getShaderManager()
	{
		return mpRenderCore->getShaderManager();
	}

	int EngineCore::getWindowHeight() { return mpRenderCore->getWindow()->getHeight(); }
	int EngineCore::getWindowWidth() { return mpRenderCore->getWindow()->getWidth(); }
	
}