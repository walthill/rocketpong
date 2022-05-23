#include "RenderCore.h"
#include <RocketEngine/render/RenderCommand.h>
#include "Window.h"
#include "RocketEngine/render/shader/Shader.h"
#include "RocketEngine/render/shader/ShaderManager.h"
#include "ComponentManager.h"
#include <glm\ext\matrix_clip_space.hpp>
#include <RKTUtils\Profiling.h>
#include <RocketEngine/asset/AssetManager.h>

namespace RKTEngine
{
	using BufferType = Renderer::BufferType;
	bool RenderCore::sRenderDebugWireframes = false;
	
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
		delete mpShaderManager;
		RenderCommand::cleanupRenderer();
		delete mpWindowHandle;
	}

	bool RenderCore::initialize()
	{
		RKT_PROFILE_FUNCTION();

		if (!createWindow())
		{
			return false;
		}

		mpShaderManager = new ShaderManager();
		mpShaderManager->addShader(AssetManager::sTEXT_SHADER_ID, new Shader(AssetManager::sTEXT_SHADER_FILENAME));
		mpShaderManager->addShader(AssetManager::sSPRITE_SHADER_ID, new Shader(AssetManager::sSPRITE_SHADER_FILENAME));

		init2DShaderData();
		RenderCommand::initRenderer(mpShaderManager->getShaderByKey(AssetManager::sSPRITE_SHADER_ID));

		return true;
	}

	void RenderCore::init2DShaderData()
	{
		glm::mat4 spriteProjection = glm::ortho(0.0f, static_cast<float>(mpWindowHandle->getWidth()),
		static_cast<float>(mpWindowHandle->getHeight()), 0.0f, -1.0f, 1.0f);

		glm::mat4 uiProjection = glm::ortho(0.0f, static_cast<float>(mpWindowHandle->getWidth()), static_cast<float>(mpWindowHandle->getHeight()), 0.0f);

		mpShaderManager->useShaderByKey(AssetManager::sTEXT_SHADER_ID);
		mpShaderManager->setShaderMat4("projection", uiProjection);

		mpShaderManager->useShaderByKey(AssetManager::sSPRITE_SHADER_ID);
		mpShaderManager->setShaderMat4("projection", spriteProjection);
	}

	void RenderCore::beginRender()
	{
		RKT_PROFILE_FUNCTION();

		RenderCommand::clearColor(Color::grey);
		RenderCommand::clearBuffer(BufferType::COLOR_BUFFER);	//depth buffer not enable for now. so render order determines layering
	}	

	void RenderCore::render(ComponentManager* componentsToDraw, float deltaTime)
	{
		RKT_PROFILE_FUNCTION();

		RenderCommand::resetStats();

		RenderCommand::beginScene();
		componentsToDraw->renderComponents();
		RenderCommand::endScene();
		
		if(sRenderDebugWireframes)
		{
			RKT_PROFILE_SCOPE("Wireframe Overlays");
			RenderCommand::beginScene();
			mpWindowHandle->setWindowDrawMode(FRONT_AND_BACK, WIREFRAME);
			componentsToDraw->renderWireframes();
			RenderCommand::endScene();
			mpWindowHandle->setWindowDrawMode(FRONT_AND_BACK, FILL);
		}

#if RKT_RENDER_STATS
		logRenderStats();
#endif
	}

	void RenderCore::endRender()
	{
		RKT_PROFILE_FUNCTION();

		mpWindowHandle->swapBuffers();
	}

	void RenderCore::logRenderStats()
	{
		auto renderStats = RenderCommand::getStats();
		RKT_CORE_TRACE("Draw Calls: " + std::to_string(renderStats.drawCalls));
		RKT_CORE_TRACE("Quads: " + std::to_string(renderStats.quadCount));
		RKT_CORE_TRACE("Verts: " + std::to_string(renderStats.getVertexCount()));
		RKT_CORE_TRACE("Indices: " + std::to_string(renderStats.getIndexCount()));
	}

	bool RenderCore::createWindow()
	{
		RKT_PROFILE_FUNCTION();

		mpWindowHandle = new Window();
		if (!mpWindowHandle->initialize(1280, 720, "RocketPong", BLEND))
		{
			return false;
		}

		return true;
	}
}