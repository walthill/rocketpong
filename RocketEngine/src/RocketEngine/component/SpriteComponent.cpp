#include "SpriteComponent.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/asset/AssetManager.h"
#include "RocketEngine/render/RenderCommand.h"

namespace RKTEngine
{
	SpriteComponent::SpriteComponent(const ComponentId& id) :
		Component(id)
	{
	}

	SpriteComponent::~SpriteComponent()
	{
		cleanup();
	}

	void SpriteComponent::load()
	{
		if (!mSpriteData.mSpriteName.empty())
		{
			mSpriteData.pSprite = EngineCore::getInstance()->getAssetManager()->loadSpriteAsset(mSpriteData.mSpriteName);

			if (!mSpriteData.mTileName.empty())
			{
				std::pair<int, int> offsets = EngineCore::getInstance()->getAssetManager()->getSpriteAtlasIndex(mSpriteData.mTileName);
				mAtlasCoords = calculateAtlasCoords(offsets.first, offsets.second);
				mAtlasInitialized = true;
			}
			else
			{
				//sprite size matches stored texture size
				mSpriteData.mWidth = mSpriteData.pSprite->getWidth();
				mSpriteData.mHeight = mSpriteData.pSprite->getHeight();
			}
		}
		else
		{
			RKT_ERROR("ERROR::Sprite component missing sprite name" + mSpriteData.mSpriteName);
		}
	}


	void SpriteComponent::cleanup()
	{
	}

	Texture2D* SpriteComponent::getSprite()
	{
		return mSpriteData.pSprite;
	}

	void SpriteComponent::setData(const SpriteComponentData& data)
	{
		mSpriteData = data; 
	}

	void SpriteComponent::process(const glm::vec2& position, const glm::vec2& scale, float rotationAngle)
	{
		mRenderInfo.position = { position.x, position.y, 0.0f };
		mRenderInfo.scale = glm::vec2(scale.x * mSpriteData.mWidth, scale.y * mSpriteData.mHeight); //scale sprite based on dimensions 
		mRenderInfo.rotation = rotationAngle;
	}

	void SpriteComponent::render()
	{
		if (mIsEnabled)
		{
			if (mRenderInfo.rotation != 0)
			{
				if(mAtlasInitialized)
					RenderCommand::drawRotatedQuad(mRenderInfo.position, mRenderInfo.scale, mRenderInfo.rotation, mSpriteData.pSprite, mAtlasCoords, 1.0f, mSpriteData.mColor);
				else
					RenderCommand::drawRotatedQuad(mRenderInfo.position, mRenderInfo.scale, mRenderInfo.rotation, mSpriteData.pSprite, 1.0f, mSpriteData.mColor);
			}
			else
			{
				if(mAtlasInitialized)
					RenderCommand::drawQuad(mRenderInfo.position, mRenderInfo.scale, mSpriteData.pSprite, mAtlasCoords, 1.0f, mSpriteData.mColor);
				else
					RenderCommand::drawQuad(mRenderInfo.position, mRenderInfo.scale, mSpriteData.pSprite, 1.0f, mSpriteData.mColor);
			}
		}
	}

	AtlasCoordinateData SpriteComponent::calculateAtlasCoords(int x, int y)
	{
		return {
			(x * mSpriteData.mWidth) / (float)mSpriteData.pSprite->getWidth(),
			((x + 1) * mSpriteData.mWidth) / (float)mSpriteData.pSprite->getWidth(),
			((y + 1) * mSpriteData.mHeight) / (float)mSpriteData.pSprite->getHeight(),
			(y * mSpriteData.mHeight) / (float)mSpriteData.pSprite->getHeight()
		};
	}


	void SpriteComponent::setSprite(const std::string& tileName)
	{
		mSpriteData.mTileName = tileName;
		load();
	}

	void SpriteComponent::setSpriteVisible(bool show)
	{
		mIsEnabled = show;
	}
}