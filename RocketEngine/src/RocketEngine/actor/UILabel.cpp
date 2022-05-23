#include "UILabel.h"
#include "RocketEngine/asset/AssetManager.h"
#include "RocketEngine/core/EngineCore.h"

namespace RKTEngine
{
	UILabel::UILabel(const glm::vec2& pos, const std::string& text)
	{
		mpGameObject = GameObjManager->createLabel(text, pos);
		Actor::init();
	}

	UILabel::UILabel(const glm::vec2& pos, const std::string& text, int fontSize)
	{
		mpGameObject = GameObjManager->createLabel(text, pos, { 1.0f, 1.0f }, 0.0f, AssetManager::sDEFAULT_FONT, fontSize);
		Actor::init();
	}

	UILabel::UILabel(const glm::vec2& pos, const std::string& text, const std::string& fontName, int fontSize)
	{
		mpGameObject = GameObjManager->createLabel(text, pos, { 1.0f, 1.0f }, 0.0f, fontName, fontSize);
		Actor::init();
	}

	void UILabel::onMessage(RKTEngine::Message& message)
	{
	}

	void UILabel::setText(const std::string& text)
	{
		mpGameObject->getUILabel()->setText(text);
	}

	void UILabel::setSize(int fontSize)
	{
		mpGameObject->getUILabel()->setFontSize(fontSize);
	}

	void UILabel::setColor(const Color& color)
	{
		mpGameObject->getUILabel()->setColor(color);

	}
}