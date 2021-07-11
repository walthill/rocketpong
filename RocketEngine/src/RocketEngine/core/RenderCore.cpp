#include "RenderCore.h"
#include "Window.h"
#include "RocketEngine/render/shader/ShaderManager.h"
#include "ComponentManager.h"
#include <glm\ext\matrix_clip_space.hpp>

namespace RKTEngine
{
	using BufferType = Renderer::BufferType;
	std::shared_ptr<VertexArray> RenderCore::mSpriteVA = nullptr;

	RenderCore::RenderCore()
	{
		mpWindowHandle = nullptr;
		mpShaderManager = nullptr;
	}

	RenderCore::~RenderCore()
	{
		clean();
	}

	void RenderCore::clean()
	{
		mSpriteVA.reset();
		delete mpShaderManager;
		delete mpWindowHandle;
	}

	bool RenderCore::initialize()
	{
		if (!createWindow())
		{
			return false;
		}

		mpShaderManager = new ShaderManager();
		mpShaderManager->addShader("sprite", new Shader("BaseSprite.vert", "BaseSprite.frag"));

		init2DVertexData();
		init2DShaderData();

		return true;
	}

	void RenderCore::init2DVertexData()
	{
		const float SPRITE_VERTICES[] = {
			// pos      // tex
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		const BufferLayout spriteLayout = {
			{ ShaderDataType::Float4, "vertex" }
		};

		std::shared_ptr<VertexBuffer> mSpriteVB;

		mSpriteVA.reset(VertexArray::create());
		mSpriteVB.reset(VertexBuffer::create(SPRITE_VERTICES, sizeof(SPRITE_VERTICES)));
		mSpriteVB->setLayout(spriteLayout);
		mSpriteVA->addVertexBuffer(mSpriteVB);
		mSpriteVA->processVertexBuffers();
	}

	void RenderCore::init2DShaderData()
	{
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(mpWindowHandle->getWidth()),
			static_cast<float>(mpWindowHandle->getHeight()), 0.0f, -1.0f, 1.0f);

		mpShaderManager->useShaderByKey("sprite");
		mpShaderManager->setShaderMat4("projection", projection);
	}

	void RenderCore::beginRender()
	{
		RenderCommand::clearColor(Color::grey);
		RenderCommand::clearBuffer(BufferType::COLOR_BUFFER);
	}

	void RenderCore::render(ComponentManager* componentsToDraw)
	{
		componentsToDraw->renderSprites();
	}

	void RenderCore::endRender()
	{
		mpWindowHandle->swapBuffers();
	}

	bool RenderCore::createWindow()
	{
		mpWindowHandle = new Window();
		if (!mpWindowHandle->initialize(1280, 720, "RogueOne"))
		{
			return false;
		}

		return true;
	}
}