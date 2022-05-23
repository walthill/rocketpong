#include "TextComponent.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/core/Log.h"
#include "RocketEngine/render/shader/ShaderManager.h"
#include <glm\ext\matrix_transform.hpp>

namespace RKTEngine
{

	TextComponent::TextComponent(const ComponentId& id) :
		Component(id)
	{
	}

	TextComponent::~TextComponent()
	{
		delete mTextData.mTextInfo;
	}

	void TextComponent::load()
	{
		if (!mTextData.mFontName.empty())
		{
			auto fontData = EngineCore::getInstance()->getAssetManager()->loadFontAsset(mTextData.mFontName);
			mTextData.mTextInfo = Text::create(fontData, mTextData.mText, mTextData.mFontSize);
		}
		else
		{
			RKT_CORE_ERROR("Font name is empty!");
		}
	}

	void TextComponent::process(const glm::vec2& position, const glm::vec2& scale, float rotationAngle)
	{
		mModelMatrix = glm::mat4(1.0f);
		mModelMatrix = glm::translate(mModelMatrix, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
		
		if (rotationAngle != 0.0f)
		{
			mModelMatrix = glm::translate(mModelMatrix, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f)); // move origin of rotation to center of quad
			mModelMatrix = glm::rotate(mModelMatrix, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
			mModelMatrix = glm::translate(mModelMatrix, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f)); // move origin back	}
		}

		mModelMatrix = glm::scale(mModelMatrix, glm::vec3(scale, 1.0f));

		mTextData.mTextInfo->process(position);
	}

	void TextComponent::render()
	{
		const auto& shaderManager = EngineCore::getInstance()->getShaderManager();
		shaderManager->useShaderByKey(mSHADER_ID);
		shaderManager->setShaderMat4(mMODEL_UNIFORM, mModelMatrix);
		shaderManager->setShaderVec3(mTEXT_COLOR_UNIFORM, mTextData.mColor.getColor01());

		mTextData.mTextInfo->renderText();
	}

	void TextComponent::setData(const TextData& data)
	{
		mTextData.mTextInfo = data.mTextInfo;
		mTextData.mText = data.mText;
		mTextData.mColor = data.mColor;
		mTextData.mFontSize = data.mFontSize;
		mTextData.mFontName = data.mFontName;
	}

	void TextComponent::setText(const std::string& text)
	{
		if (mTextData.mText.compare(text) != 0)
		{
			mTextData.mText = text;
			mTextData.mTextInfo->setText(text, mTextData.mFontSize);
		}
	}
	void TextComponent::setColor(const Color& color)
	{
		mTextData.mColor = color;
	}
	void TextComponent::setFontSize(int fontSize)
	{
		mTextData.mFontSize = fontSize;
		mTextData.mTextInfo->setText(mTextData.mText, fontSize);
	}

}