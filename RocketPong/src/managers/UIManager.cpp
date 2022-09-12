#include "UIManager.h"
#include "GameApp.h"

REGISTER_ACTOR(UIManager);

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
