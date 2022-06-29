#include "UILabel.h"
#include "RocketEngine/asset/AssetManager.h"
#include "RocketEngine/core/EngineCore.h"

namespace RKTEngine
{
	UILabel::UILabel(const glm::vec2& pos, const std::string& text)
	{
		EngineCore::getInstance()->getSceneManager()->registerUI(this);
		mpGameObject = GameObjManager->createLabel(text, pos);
		Actor::init(true);
	}

	UILabel::UILabel(const glm::vec2& pos, const std::string& text, int fontSize)
	{
		mpGameObject = GameObjManager->createLabel(text, pos, { 1.0f, 1.0f }, 0.0f, AssetManager::sDEFAULT_FONT, fontSize);
		Actor::init(true);
	}

	UILabel::UILabel(const glm::vec2& pos, const std::string& text, const std::string& fontName, int fontSize)
	{
		mpGameObject = GameObjManager->createLabel(text, pos, { 1.0f, 1.0f }, 0.0f, fontName, fontSize);
		Actor::init(true);
	}

	void UILabel::onMessage(Message& message)
	{
	}

	std::string UILabel::getText()
	{
		return getTextInternal();
	}
	std::string UILabel::getFontName()
	{
		return getFontNameInternal();
	}
	int  UILabel::getFontSize()
	{
		return getFontSizeInternal();
	}
	void UILabel::setText(int text)
	{
		setTextInternal(std::to_string(text));
	}
	void UILabel::setText(float text)
	{
		setTextInternal(std::to_string(text));
	}
	glm::vec4 UILabel::getColor()
	{
		return getColorInternal();
	}
	void UILabel::setText(const std::string& text)
	{
		setTextInternal(text);
	}
	void UILabel::setFontSize(int fontSize)
	{
		setFontSizeInternal(fontSize);
	}
	void UILabel::setFont(const std::string& fontName)
	{
		setFontInternal(fontName);
	}
	void UILabel::setColor(const Color& color)
	{
		setColorInternal(color);
	}


	std::string UILabel::getTextInternal()
	{
		return mpGameObject->getUILabel()->getData().mText;
	}
	glm::vec4 UILabel::getColorInternal()
	{
		return mpGameObject->getUILabel()->getData().mColor.getColorAlpha();
	}
	std::string UILabel::getFontNameInternal()
	{
		return mpGameObject->getUILabel()->getData().mFontName;
	}
	int UILabel::getFontSizeInternal()
	{
		return mpGameObject->getUILabel()->getData().mFontSize;
	}
	void UILabel::setTextInternal(const std::string& text)
	{
		mpGameObject->getUILabel()->setText(text);
	}
	void UILabel::setFontSizeInternal(int fontSize)
	{
		mpGameObject->getUILabel()->setFontSize(fontSize);
	}
	void UILabel::setFontInternal(const std::string& fontName)
	{
		mpGameObject->getUILabel()->getData().mFontName = fontName;
	}
	void UILabel::setColorInternal(const Color& color)
	{
		mpGameObject->getUILabel()->setColor(color);
	}
}