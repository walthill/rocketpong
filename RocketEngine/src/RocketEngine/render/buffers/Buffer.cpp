#include "Buffer.h"
#include "RocketEngine/render/renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace RKTEngine
{

	VertexBuffer* VertexBuffer::create(uint32 size, DataType usage)
	{
		switch (Renderer::getAPI())
		{
			case Renderer::API::NONE:		return nullptr;
			case Renderer::API::OPENGL:		return new OpenGLVertexBuffer(size, usage);
		}
		return nullptr;
	}

	VertexBuffer* VertexBuffer::create(const float* vertices, uint32 size, DataType usage)
	{
		switch (Renderer::getAPI())
		{
		case Renderer::API::NONE:		return nullptr;
		case Renderer::API::OPENGL:		return new OpenGLVertexBuffer(vertices, size, usage);
		}
		return nullptr;
	}

	VertexBuffer* VertexBuffer::create(const void* data, uint32 size, DataType usage)
	{
		switch (Renderer::getAPI())
		{
		case Renderer::API::NONE:		return nullptr;
		case Renderer::API::OPENGL:		return new OpenGLVertexBuffer(data, size, usage);
		}
		return nullptr;
	}


	IndexBuffer* IndexBuffer::create(uint32* indicies, uint32 count)
	{
		switch (Renderer::getAPI())
		{
		case Renderer::API::NONE:		return nullptr;
		case Renderer::API::OPENGL:		return new OpenGLIndexBuffer(indicies, count);
		}
		return nullptr;
	}

	FrameBuffer* FrameBuffer::create(int width, int height, int aaSamples)
	{
		switch (Renderer::getAPI())
		{
		case Renderer::API::NONE:		return nullptr;
		case Renderer::API::OPENGL:		return new OpenGLFrameBuffer(width, height, aaSamples);
		}
		return nullptr;
	}
}