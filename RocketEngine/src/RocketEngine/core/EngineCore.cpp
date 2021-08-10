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
		

		mpComponentManager = new ComponentManager(1000);
		mpEntityManager = new GameObjectManager(1000);

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
		mpInputSystem = new InputSystem(mpRenderCore->getWindow());
		mpMessageManager = new MessageManager();
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

	ShaderManager* EngineCore::getShaderManager()
	{
		return mpRenderCore->getShaderManager();
	}

}