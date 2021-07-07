#include "VertexArray.h"
#include "RocketEngine/core/RenderCore.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace RKTEngine
{
	VertexArray* VertexArray::create()
	{
		switch (RenderCore::getAPI())
		{
			case Renderer::API::NONE:		break;
			case Renderer::API::OPENGL:		return new OpenGLVertexArray();
		}

		RKT_ASSERT(false);	//unknown api
		return nullptr;
	}
}