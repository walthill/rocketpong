#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRenderer.h"

namespace RKTEngine
{
	Renderer* RenderCommand::spRendererAPI = new OpenGLRenderer;
}