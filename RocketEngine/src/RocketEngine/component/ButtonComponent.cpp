#include "ButtonComponent.h"
#include <glm\ext\matrix_transform.hpp>
#include <RocketEngine/core/Log.h>
#include <RocketEngine/core/EngineCore.h>
#include <RocketEngine/asset/AssetManager.h>
#include <RocketEngine/render/shader/ShaderManager.h>
#include <RocketEngine/render/RenderCommand.h>

namespace RKTEngine
{
	ButtonComponent::ButtonComponent(const ComponentId& id) :
		Component(id)
	{
	}

	ButtonComponent::~ButtonComponent()
	{
		cleanup();
	}

	void ButtonComponent::load()
	{
		//load sprites
		loadSprite(mData.mSprite);
		loadSprite(mData.mHighlightSprite, true);

		//load text
		loadText();
	}

	void ButtonComponent::cleanup()
	{
		mData.mCallbackVoid = nullptr;
		mData.mCallbackBool = nullptr;
		mData.mCallbackInt = nullptr;
		delete mData.mText.mTextInfo;
	}

	const std::string& ButtonComponent::getText()
	{
		return mData.mText.mText;
	}

	void ButtonComponent::setText(const std::string& text)
	{
		if (mData.mText.mText.compare(text) != 0)
		{
			mData.mText.mText = text;
			mData.mText.mTextInfo->setText(text, mData.mText.mFontSize);
		}
	}

	void ButtonComponent::setText(int text)
	{
		auto str = std::to_string(text);
		mData.mText.mText = str;
		mData.mText.mTextInfo->setText(str, mData.mText.mFontSize);
	}

	void ButtonComponent::setFont(const std::string& font)
	{
		if (!font.empty() && mData.mText.mFontName.compare(font) != 0)
		{
			mData.mText.mFontName = font;
			loadText();
		}
	}

	void ButtonComponent::setData(const ButtonComponentData& data)
	{
		mData.mSprite = data.mSprite;
		mData.mHighlightSprite = data.mHighlightSprite;
		mData.mText = data.mText;
		mData.isEnabled = data.isEnabled;
	}

	void ButtonComponent::processSprite(const glm::vec2& position, const glm::vec2& scale, float rotationAngle)
	{
		if (!mData.isEnabled)
			return;

		if (mData.mHighlighted)
		{
			processSprite(mData.mHighlightSprite, position, scale, rotationAngle);
		}
		else
		{
			processSprite(mData.mSprite, position, scale, rotationAngle);
		}
	}

	void ButtonComponent::processText(const glm::vec2& position, const glm::vec2& scale, float rotationAngle)
	{
		if (!mData.isEnabled)
			return;

		mModelMatrix = glm::mat4(1.0f);
		mModelMatrix = glm::translate(mModelMatrix, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

		if (rotationAngle != 0.0f)
		{
			mModelMatrix = glm::translate(mModelMatrix, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f)); // move origin of rotation to center of quad
			mModelMatrix = glm::rotate(mModelMatrix, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
			mModelMatrix = glm::translate(mModelMatrix, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f)); // move origin back	}
		}

		mModelMatrix = glm::scale(mModelMatrix, glm::vec3(scale, 1.0f));

		mData.mText.mTextInfo->process(position);
	}

	void ButtonComponent::renderSprite()
	{
		if (mData.isEnabled)
		{
			if (mRenderInfo.rotation != 0)
			{
				if (mData.mHighlighted)
				{
					if(mHighlightAtlasInitialized)
						RenderCommand::drawRotatedQuad(mRenderInfo.position, mRenderInfo.scale, mRenderInfo.rotation, mData.mHighlightSprite.pSprite, mHighlightAtlasCoords, 1.0f, mData.mHighlightSprite.mColor);
					else
						RenderCommand::drawRotatedQuad(mRenderInfo.position, mRenderInfo.scale, mRenderInfo.rotation, mData.mHighlightSprite.pSprite, 1.0f, mData.mHighlightSprite.mColor);
				}
				else
				{
					if(mAtlasInitialized)
						RenderCommand::drawRotatedQuad(mRenderInfo.position, mRenderInfo.scale, mRenderInfo.rotation, mData.mSprite.pSprite, mAtlasCoords, 1.0f, mData.mSprite.mColor);
					else
						RenderCommand::drawRotatedQuad(mRenderInfo.position, mRenderInfo.scale, mRenderInfo.rotation, mData.mSprite.pSprite, 1.0f, mData.mSprite.mColor);
				}
			}
			else
			{
				if (mData.mHighlighted)
				{
					if (mHighlightAtlasInitialized)
						RenderCommand::drawQuad(mRenderInfo.position, mRenderInfo.scale, mData.mHighlightSprite.pSprite, mHighlightAtlasCoords, 1.0f, mData.mHighlightSprite.mColor);
					else
						RenderCommand::drawQuad(mRenderInfo.position, mRenderInfo.scale, mData.mHighlightSprite.pSprite, 1.0f, mData.mHighlightSprite.mColor);
				}
				else
				{
					if(mAtlasInitialized)
						RenderCommand::drawQuad(mRenderInfo.position, mRenderInfo.scale, mData.mSprite.pSprite, mAtlasCoords, 1.0f, mData.mSprite.mColor);
					else
						RenderCommand::drawQuad(mRenderInfo.position, mRenderInfo.scale, mData.mSprite.pSprite, 1.0f, mData.mSprite.mColor);
				}
			}
		}
	}
	
	void ButtonComponent::renderText()
	{
		if (!mData.isEnabled)
			return;

		const auto& shaderManager = EngineCore::getInstance()->getShaderManager();
		shaderManager->useShaderByKey(mSHADER_ID);
		shaderManager->setShaderMat4(mMODEL_UNIFORM, mModelMatrix);
		shaderManager->setShaderVec3(mTEXT_COLOR_UNIFORM, mData.mText.mColor.getColor01());
		mData.mText.mTextInfo->renderText();
	}
	
	AtlasCoordinateData ButtonComponent::calculateAtlasCoords(SpriteComponentData& sprData, int x, int y)
	{
		return {
			(x * sprData.mWidth) / (float)sprData.pSprite->getWidth(),
			((x + 1) * sprData.mWidth) / (float)sprData.pSprite->getWidth(),
			((y + 1) * sprData.mHeight) / (float)sprData.pSprite->getHeight(),
			(y * sprData.mHeight) / (float)sprData.pSprite->getHeight()
		};
	}

	void ButtonComponent::loadSprite(SpriteComponentData& sprData, bool isHighlightSpr)
	{
		if (!sprData.mSpriteName.empty())
		{
			sprData.pSprite = EngineCore::getInstance()->getAssetManager()->loadSpriteAsset(sprData.mSpriteName);

			if (!sprData.mTileName.empty())
			{
				std::pair<int, int> offsets = EngineCore::getInstance()->getAssetManager()->getSpriteAtlasIndex(sprData.mTileName);
				if (isHighlightSpr)
				{
					mHighlightAtlasCoords = calculateAtlasCoords(sprData, offsets.first, offsets.second);
					mHighlightAtlasInitialized = true;
				}
				else
				{
					mAtlasCoords = calculateAtlasCoords(sprData, offsets.first, offsets.second);
					mAtlasInitialized = true;
				}
			}
			else
			{
				//sprite size matches stored texture size
				sprData.mWidth = sprData.pSprite->getWidth();
				sprData.mHeight = sprData.pSprite->getHeight();
			}
		}
		else
		{
			RKT_ERROR("'{0}' Sprite component missing sprite name" + sprData.mSpriteName, LOG_SUBSYS(ButtonComponent));
		}
	}

	void ButtonComponent::loadText()
	{
		if (!mData.mText.mFontName.empty())
		{
			auto fontData = EngineCore::getInstance()->getAssetManager()->loadFontAsset(mData.mText.mFontName);
			
			if (mData.mText.mTextInfo != nullptr)
				delete mData.mText.mTextInfo;

			mData.mText.mTextInfo = Text::create(fontData, mData.mText.mText, mData.mText.mFontSize);
		}
		else
		{
			RKT_CORE_ERROR("'{0}' Font name is empty!", LOG_SUBSYS(ButtonComponent));
		}
	}
	void ButtonComponent::processSprite(SpriteComponentData& sprData, const glm::vec2& position, const glm::vec2& scale, float rotationAngle)
	{
		if (!mData.isEnabled)
			return;

		mRenderInfo.position = { position.x, position.y, 0.0f };
		mRenderInfo.scale = glm::vec2(scale.x * sprData.mWidth, scale.y * sprData.mHeight); //scale sprite based on dimensions 
		mRenderInfo.rotation = rotationAngle;
	}


	void ButtonComponent::onSelected()
	{
		if(mData.mCallbackVoid != nullptr && mData.isEnabled)
			mData.mCallbackVoid();
	}

	void ButtonComponent::setHighlighted(bool highlighted)
	{
		mData.mHighlighted = highlighted;
	}

	void ButtonComponent::setOnSelected(ButtonCallbackVoid func)
	{
		mData.mCallbackVoid = func;
	}
}