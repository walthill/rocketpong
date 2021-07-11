#include "SpriteComponent.h"
#include "RocketEngine/render/shader/Shader.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/core/RenderCore.h"
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
		if (mSpriteData.mSpriteName != "null" && mSpriteData.mSpriteName != "")
		{
			std::string folderName = mSpriteData.mSpriteName.substr(0, mSpriteData.mSpriteName.find('.'));
			mSpriteData.sprite = EngineCore::getInstance()->getAssetManager()->loadSpriteAsset(modelFileLocation + mSpriteData.mSpriteName);
			mSpriteData.shader->use();
			mSpriteData.shader->setInt("image", 0);
		}
	}


	void SpriteComponent::cleanup()
	{
	}

	Texture2D* SpriteComponent::getSprite()
	{
		return mSpriteData.sprite;
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
		if (mSpriteData.shader != nullptr)
		{
			mSpriteData.shader->use();
			if (mSpriteData.instanceCount == 1)
			{	
				mSpriteData.shader->setMat4(mMODEL_MATRIX_ID, mModelMatrix);
				mSpriteData.shader->setVec3("spriteColor", mSpriteData.mColor.getColor());
			}

			if (mIsEnabled && mSpriteData.sprite != nullptr)
			{
				RenderCommand::setActiveTexture(Renderer::TextureChannel::TEX_CHANNEL0);
				attatchSpriteData();
				RenderCommand::drawSprite(RenderCore::getSpriteVertexData(), 1);
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