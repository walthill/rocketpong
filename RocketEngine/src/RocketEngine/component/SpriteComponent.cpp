#include "SpriteComponent.h"
#include "RocketEngine/render/shader/Shader.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/core/RenderCore.h"
#include "RocketEngine/render/shader/ShaderManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>

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
			mSpriteData.sprite = EngineCore::getInstance()->getAssetManager()->loadSpriteAsset(mSpriteData.mSpriteName);
		}
		else
		{
			RKT_ERROR("Sprite component missing sprite name");
		}
	}


	void SpriteComponent::cleanup()
	{
	}

	Texture2D* SpriteComponent::getSprite()
	{
		return mSpriteData.sprite;
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
		scale = glm::vec2(scale.x * mSpriteData.sprite->getWidth(), scale.y * mSpriteData.sprite->getHeight());

		mModelMatrix = glm::mat4(1.0f);
		mModelMatrix = glm::translate(mModelMatrix, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

		mModelMatrix = glm::translate(mModelMatrix, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f)); // move origin of rotation to center of quad
		mModelMatrix = glm::rotate(mModelMatrix, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
		mModelMatrix = glm::translate(mModelMatrix, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f)); // move origin back	}
		mModelMatrix = glm::scale(mModelMatrix, glm::vec3(scale, 1.0f)); // last scale
	}

	void SpriteComponent::render()
	{
		if (mSpriteData.mpShader != nullptr)
		{
			mSpriteData.mpShader->use();
			if (mSpriteData.instanceCount == 1)
			{	
				mSpriteData.mpShader->setMat4(mMODEL_MATRIX_ID, mModelMatrix);
				mSpriteData.mpShader->setVec3(mSPRITE_COLOR_ID, mSpriteData.mColor.getColor01());
			}

			if (mIsEnabled && mSpriteData.sprite != nullptr)
			{
				RenderCommand::setActiveTexture(Renderer::TextureChannel::TEX_CHANNEL0);
				attatchSpriteData();
				RenderCore::submit(RenderCore::getSpriteVertexData());
			}
		}
	}

	//map sprite data to the gpu
	void SpriteComponent::attatchSpriteData()
	{
		mSpriteData.sprite->bind();
	}

	void SpriteComponent::setSpriteVisible(bool show)
	{
		mIsEnabled = show;
	}
}