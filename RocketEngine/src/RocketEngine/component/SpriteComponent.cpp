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
			mSpriteData.sprite = EngineCore::getInstance()->getAssetManager()->loadSpriteAsset(mSpriteData.mSpriteName);

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
		if (mSpriteData.mpShader != nullptr)
		{
			mSpriteData.mpShader->use();
			auto va = EngineCore::getInstance()->getRenderer()->getSpriteVertexData();
			auto vb = EngineCore::getInstance()->getRenderer()->getSpriteBufferData();

			if (mSpriteData.instanceCount == 1)
			{	
				//Set atlas coordinates
				float leftX = (mAtlasOffsetX * mSpriteData.mWidth) / (float)mSpriteData.sprite->getWidth();
				float rightX = ((mAtlasOffsetX + 1) * mSpriteData.mWidth) / (float)mSpriteData.sprite->getWidth();
				float topY = ((mAtlasOffsetY + 1) * mSpriteData.mHeight) / (float)mSpriteData.sprite->getHeight();
				float bottomY = (mAtlasOffsetY * mSpriteData.mHeight) / (float)mSpriteData.sprite->getHeight();
				
				/* **** SPRITE ATLAS NOTES ****
					bottom left coord
						x = (x offset in atlas * width of sprite) / atlaswidth
						y = (y offset in atlas * heigh of sprite) / atlas height

					bottom right coord
						x = ((x offset in atlas + 1) * width of sprite) / atlas width
						y = (y offset in atlas * heigh of sprite) / atlas height

					top right coord
						x = (x offset in atlas + 1) * width of sprite	
						y = ((y offset in atlas + 1) * heigh of sprite) / atlas height

					top left coord
						x = x offset in atlas * width of sprite
						y = ((y offset in atlas + 1) * heigh of sprite) / atlas height
				*/

				float SPRITE_VERTICES[] = {
					// pos			// tex coords
							//tri #1
					0.0f, 1.0f,		leftX, topY,		//top left					
					1.0f, 0.0f,		rightX, bottomY,	//bottom right
					0.0f, 0.0f,		leftX, bottomY,		//bottom left

							//tri #2
					0.0f, 1.0f,		leftX, topY,		//top left
					1.0f, 1.0f,		rightX, topY,		//top right
					1.0f, 0.0f,		rightX, bottomY		//bottom right
				};

				const BufferLayout spriteLayout = {
					{ ShaderDataType::Float4, "vertex" }
				};

				vb.reset(VertexBuffer::create(SPRITE_VERTICES, sizeof(SPRITE_VERTICES)));
				vb->setLayout(spriteLayout);

				va->clearVertexBuffers();
				va->addVertexBuffer(vb);
				va->processVertexBuffers();

				//Set matrix data
				mSpriteData.mpShader->setMat4(mMODEL_MATRIX_ID, mModelMatrix);
				mSpriteData.mpShader->setVec3(mSPRITE_COLOR_ID, mSpriteData.mColor.getColor01());
			}

			if (mIsEnabled && mSpriteData.sprite != nullptr)
			{
				RenderCommand::setActiveTexture(Renderer::TextureChannel::TEX_CHANNEL0);
				attatchSpriteData();
				RenderCore::submit(va);
			}
		}
	}

	//map sprite data to the gpu
	void SpriteComponent::attatchSpriteData()
	{
		mSpriteData.sprite->bind();
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