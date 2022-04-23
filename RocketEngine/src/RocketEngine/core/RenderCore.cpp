#include "RenderCore.h"
#include <RocketEngine/render/RenderCommand.h>
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
		delete tex;
		mSpriteVA.reset();
		delete mpShaderManager;
		RenderCommand::cleanupRenderer();
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
		mpShaderManager->addShader("batchtex", new Shader("BatchTexture.vert", "BatchTexture.frag"));

		init2DVertexData();
		init2DShaderData();
		RenderCommand::initRenderer(mpShaderManager->getShaderByKey("batchtex"));

		tex = Texture2D::create("assets/sprites/smiles.png");

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

		mpShaderManager->useShaderByKey("batchtex");
		mpShaderManager->setShaderMat4("projection", spriteProjection);
	}

	void RenderCore::beginRender()
	{
		RenderCommand::clearColor(Color::grey);
		RenderCommand::clearBuffer(BufferType::COLOR_BUFFER | BufferType::DEPTH_BUFFER);
	}	

	void RenderCore::render(ComponentManager* componentsToDraw, float deltaTime)
	{		
		static float rot = 0;
		rot += deltaTime * 50;
		RenderCommand::beginScene();

		RenderCommand::drawQuad({ 220, 250, 0 }, { 64, 64 }, { 1.0f,.2f,.3f, 0.25f }); //TODO: fix alpha blending against other obj
		RenderCommand::drawQuad({ 350, 300, 0 }, { 64, 64 }, { 0.2f,.3f,.8f, 1.0f });
		//RenderCommand::drawQuad({ 500, 290, -1 }, { 64, 64 }, tex, 1.0f);	//TODO: fix white artifacts
		//RenderCommand::drawQuad({ 355, 400 }, { 64, 64 }, tex, 1.0f);
		RenderCommand::drawRotatedQuad({ 500, 200, 0 }, { 512 / 4, 512 / 4}, rot, tex, 7.0f);

		componentsToDraw->renderComponents();
		RenderCommand::endScene();
	}

	void RenderCore::endRender()
	{
		mpWindowHandle->swapBuffers();
	}

	bool RenderCore::createWindow()
	{
		mpWindowHandle = new Window();
		if (!mpWindowHandle->initialize(1280, 720, "RogueOne", BLEND | DEPTH_TEST))
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