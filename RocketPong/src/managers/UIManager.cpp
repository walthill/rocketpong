#include "UIManager.h"
#include "GameApp.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

void UIManager::onStart()
{
	GameObjManager->setButtonCallback(playButtonId, RKT_BIND_CALLBACK_FN(UIManager::onStartSelected));
	GameObjManager->setButtonCallback(controlsButtonId, RKT_BIND_CALLBACK_FN(UIManager::onControlsSelected));
	GameObjManager->setButtonCallback(controlsBackButtonId, RKT_BIND_CALLBACK_FN(UIManager::onControlsBackSelected));
	GameObjManager->setButtonCallback(exitButtonId, RKT_BIND_CALLBACK_FN(UIManager::onQuitSelected));
}

void UIManager::onQuitSelected()
{
	GameApp::getInstance()->quit(RKTEngine::ExitMessage());
}

void UIManager::onStartSelected(void)
{
	RocketEngine->getSceneManager()->loadScene("game");
}

void UIManager::onControlsSelected()
{
	RocketEngine->getSceneManager()->loadScene("controls");
}

void UIManager::onControlsBackSelected()
{
	RocketEngine->getSceneManager()->loadScene("main");
}

void UIManager::onDeserialize(int id)
{
	auto obj = GameObjManager->findGameObject(id);
	if (obj != nullptr)
	{
		auto scriptComp = obj->getNativeScript();
		scriptComp->bind<UIManager>(id);
		auto inst = scriptComp->get<UIManager>();

		//restore serialized values here
		inst->playButtonId = playButtonId;
		inst->exitButtonId = exitButtonId;
		inst->controlsButtonId = controlsButtonId;
		inst->controlsBackButtonId = controlsBackButtonId;
	}
}
