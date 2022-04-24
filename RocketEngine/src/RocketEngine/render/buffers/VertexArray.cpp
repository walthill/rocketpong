#include "VertexArray.h"
#include "RocketEngine/render/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace RKTEngine
{
	VertexArray* VertexArray::create()
	{
		switch (Renderer::getAPI())
		{
			case Renderer::API::NONE:		break;
			case Renderer::API::OPENGL:		return new OpenGLVertexArray();
		}

		RKT_ASSERT(false);	//unknown api
		return nullptr;
	}
}