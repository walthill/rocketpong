#include "RenderCore.h"
#include "Window.h"
#include "RocketEngine/render/shader/ShaderManager.h"
#include "ComponentManager.h"
#include <glm\ext\matrix_clip_space.hpp>

namespace RKTEngine
{
	using BufferType = Renderer::BufferType;
	
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
		mpShaderManager->addShader("text", new Shader("TextRender.vert", "TextRender.frag"));

		init2DVertexData();
		init2DShaderData();

		return true;
	}

	void RenderCore::init2DVertexData()
	{		
		mSpriteVA.reset(VertexArray::create());
	}

	void RenderCore::init2DShaderData()
	{
		glm::mat4 spriteProjection = glm::ortho(0.0f, static_cast<float>(mpWindowHandle->getWidth()),
		static_cast<float>(mpWindowHandle->getHeight()), 0.0f, -1.0f, 1.0f);

		glm::mat4 uiProjection = glm::ortho(0.0f, static_cast<float>(mpWindowHandle->getWidth()), static_cast<float>(mpWindowHandle->getHeight()), 0.0f);

		mpShaderManager->useShaderByKey("sprite");
		mpShaderManager->setShaderMat4("projection", spriteProjection);

		mpShaderManager->useShaderByKey("text");
		mpShaderManager->setShaderMat4("projection", uiProjection);
	}

	void RenderCore::beginRender()
	{
		RenderCommand::clearColor(Color::grey);
		RenderCommand::clearBuffer(BufferType::COLOR_BUFFER);
	}

	void RenderCore::render(ComponentManager* componentsToDraw)
	{
		componentsToDraw->renderComponents();
	}

	void RenderCore::endRender()
	{
		mpWindowHandle->swapBuffers();
	}

	bool RenderCore::createWindow()
	{
		mpWindowHandle = new Window();
		if (!mpWindowHandle->initialize(1280, 720, "RogueOne", BLEND))
		{
			return false;
		}

		return true;
	}

	void RenderCore::submit(const std::shared_ptr<VertexArray>& vertexArray, int instanceCount)
	{
		vertexArray->bind();
		if (vertexArray->getIndexBuffer() != nullptr)
			RenderCommand::drawIndexed(vertexArray);
		else
			RenderCommand::drawSprite(vertexArray, instanceCount);
	}
}