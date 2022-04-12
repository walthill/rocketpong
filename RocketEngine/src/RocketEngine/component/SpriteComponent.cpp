#include "SpriteComponent.h"
#include "RocketEngine/render/shader/Shader.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/render/RenderCommand.h"
#include "RocketEngine/render/shader/ShaderManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>

namespace RKTEngine
{
	SpriteComponent::SpriteComponent(const ComponentId& id) :
		Component(id), mAtlasOffsetX(0), mAtlasOffsetY(0)
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
				mAtlasOffsetX = offsets.first;
				mAtlasOffsetY = offsets.second;
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
		const std::string& spriteShaderID = EngineCore::getInstance()->getAssetManager()->getSpriteShaderID();
		mSpriteData.mpShader = EngineCore::getInstance()->getShaderManager()->getShaderByKey(spriteShaderID);
	}

	void SpriteComponent::process(glm::vec2 position, glm::vec2 scale, float rotationAngle)
	{
		//scale sprite based on dimensions
		//allows scale vector to modify existing sprite and maintain aspect ration, dimension, etc
		scale = glm::vec2(scale.x * mSpriteData.mWidth, scale.y * mSpriteData.mHeight);

		mModelMatrix = glm::mat4(1.0f);
		mModelMatrix = glm::translate(mModelMatrix, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

		mModelMatrix = glm::translate(mModelMatrix, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f)); // move origin of rotation to center of quad
		mModelMatrix = glm::rotate(mModelMatrix, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
		mModelMatrix = glm::translate(mModelMatrix, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f)); // move origin back	}
		mModelMatrix = glm::scale(mModelMatrix, glm::vec3(scale, 1.0f)); // last scale
	}

	void SpriteComponent::render()
	{
		if (mIsEnabled && mSpriteData.pSprite != nullptr)
		{
			//auto scale = glm::vec2(scale.x * mSpriteData.mWidth, scale.y * mSpriteData.mHeight);
			//RenderCommand::drawQuad(position, scale, mSpriteData.pSprite, calculateAtlasCoords(), 1.0f);
		}
	}

	AtlasCoordinateData SpriteComponent::calculateAtlasCoords()
	{
		return {
			(mAtlasOffsetX * mSpriteData.mWidth) / (float)mSpriteData.pSprite->getWidth(),
			((mAtlasOffsetX + 1) * mSpriteData.mWidth) / (float)mSpriteData.pSprite->getWidth(),
			((mAtlasOffsetY + 1) * mSpriteData.mHeight) / (float)mSpriteData.pSprite->getHeight(),
			(mAtlasOffsetY * mSpriteData.mHeight) / (float)mSpriteData.pSprite->getHeight()
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