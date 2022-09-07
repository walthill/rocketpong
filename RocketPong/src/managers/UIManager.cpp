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
	GameObjManager->setButtonCallback(exitButtonId, RKT_BIND_CALLBACK_FN(UIManager::onQuitSelected));
}

void UIManager::onQuitSelected()
{
	GameApp::getInstance()->quit(RKTEngine::ExitMessage());
}

void UIManager::onStartSelected(void)
{
	std::cout << "BUTTON SELECTED WOO" << std::endl;
}

void UIManager::onDeserialize(int id)
{
	auto obj = GameObjManager->findGameObject(id);
	if (obj != nullptr)
	{
		auto script = obj->getScript();
		script->bind<UIManager>(id);
		auto inst = script->get<UIManager>();

		//restore serialized values here
		inst->playButtonId = playButtonId;
		inst->exitButtonId = exitButtonId;
	}
}
