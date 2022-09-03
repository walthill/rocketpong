#include "EngineCore.h"
#include "RenderCore.h"
#include "Window.h"
#include "Log.h"
#include "InputSystem.h"
#include "MessageManager.h"
#include "RocketEngine/render/shader/ShaderManager.h"
#include "ComponentManager.h"
#include "RocketEngine/gameobject/GameObjectManager.h"
#include "RocketEngine/asset/AssetManager.h"
#include "RocketEngine/asset/scene/SceneManager.h"
#include "AudioManager.h"

namespace RKTEngine
{
	float EngineCore::sDeltaTime = 0;
	
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
		delete mpSceneManager;
		delete mpEntityManager;
		delete mpAssetManager;
		delete mpComponentManager;
		delete mpMessageManager;
		delete mpInputSystem;
		delete mpAudioManager;
		delete mpRenderCore;
	}

	bool EngineCore::initialize()
	{
		mpMasterTimer = new RKTUtil::Timer();
		sDeltaTime = 0.0f;
		
		if (!initRenderCore())
		{
			return false;
		}
		initInputSystem();

		mpAssetManager = new AssetManager();
		if (!mpAssetManager->initialize())
		{
			return false;
		}

		mpComponentManager = new ComponentManager(5000);
		mpEntityManager = new GameObjectManager(5000);
		mpAudioManager = new AudioManager();
		mpSceneManager = new SceneManager();

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
		mpMessageManager->processMessagesForThisFrame(sDeltaTime);

		mpComponentManager->update(sDeltaTime);
		mpEntityManager->updateAll(sDeltaTime);

		mpInputSystem->processInput();
		calculateDeltaTime();
	}

	void EngineCore::render()
	{
		mpRenderCore->beginRender();
		mpRenderCore->render(mpComponentManager, sDeltaTime);
		mpRenderCore->endRender();
	}

	void EngineCore::onMessage(Message& message)
	{
		//TODO: update orthographic projection on window resize
		mpComponentManager->onMessage(message);
		mpAudioManager->onMessage(message);
		mpSceneManager->onMessage(message);
	}

	void EngineCore::calculateDeltaTime()
	{
		float currentFrame = (float)getTime();
		sDeltaTime = (currentFrame - mLastFrame) / 1000;
		mLastFrame = currentFrame;
	}

	double EngineCore::getTime() { return mpMasterTimer->getTimeElapsedMs(); }
	ComponentManager* EngineCore::getComponentManager() { return mpComponentManager; }
	AssetManager* EngineCore::getAssetManager() { return mpAssetManager; }
	GameObjectManager* EngineCore::getEntityManager() { return mpEntityManager; }
	SceneManager* EngineCore::getSceneManager() { return mpSceneManager; }
	RenderCore* EngineCore::getRenderer() { return mpRenderCore; }
	InputSystem* EngineCore::getInputSystem() { return mpInputSystem; }
	MessageManager* EngineCore::getMessageManager() { return mpMessageManager; }

	ShaderManager* EngineCore::getShaderManager()
	{
		return mpRenderCore->getShaderManager();
	}

	void EngineCore::showDebugWireframes(bool show)
	{
		mpRenderCore->sRenderDebugWireframes = show;
	}

	int EngineCore::getWindowHeight() { return mpRenderCore->getWindow()->getHeight(); }
	int EngineCore::getWindowWidth() { return mpRenderCore->getWindow()->getWidth(); }
	
}