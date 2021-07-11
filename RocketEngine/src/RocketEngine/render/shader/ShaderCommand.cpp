#include "ShaderCommand.h"
#include "Platform/OpenGL/shader/OpenGLShader.h"

namespace RKTEngine
{
	ShaderAPI* ShaderCommand::spShaderAPI = new OpenGLShader;
}