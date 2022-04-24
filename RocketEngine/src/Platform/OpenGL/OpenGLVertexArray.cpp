#include "OpenGLVertexArray.h"
#include "RocketEngine/core/RenderCore.h"
#include <glad/glad.h>
#include <RocketEngine/render/Renderer.h>

namespace RKTEngine
{
	static uint32 ShaderDataTypeToGLType(ShaderDataType type)
	{
		if (Renderer::getAPI() == Renderer::API::OPENGL)
		{
			switch (type)
			{
				case ShaderDataType::Float:		return GL_FLOAT;
				case ShaderDataType::Float2:	return GL_FLOAT;
				case ShaderDataType::Float3:	return GL_FLOAT;
				case ShaderDataType::Float4:	return GL_FLOAT;
				case ShaderDataType::Mat3:		return GL_FLOAT;
				case ShaderDataType::Mat4:		return GL_FLOAT;
				case ShaderDataType::Int:		return GL_INT;
				case ShaderDataType::Int2:		return GL_INT;
				case ShaderDataType::Int3:		return GL_INT;
				case ShaderDataType::Int4:		return GL_INT;
				case ShaderDataType::Bool:		return GL_BOOL;
			}
		}

		RKT_ASSERT(false);
		return 0;
	}


	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &mRendererId);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glad_glDeleteVertexArrays(1, &mRendererId);
	}

	void OpenGLVertexArray::bind() const
	{
		glBindVertexArray(mRendererId);
	}

	void OpenGLVertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		//VB has no layout
		RKT_ASSERT(vertexBuffer->getLayout().GetElements().size());
		mVertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::processVertexBuffers()
	{
		glBindVertexArray(mRendererId);
		uint32 index = 0;
		for (auto& vertexBuffer : mVertexBuffers)
		{
			vertexBuffer->bind();

			auto& layout = vertexBuffer->getLayout();
			for (auto& element : layout)
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index,
					element.getComponentCount(),
					ShaderDataTypeToGLType(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					layout.getStride(),
					(const void*)element.offset);
				glVertexAttribDivisor(index, element.instanceIterations);
				index++;
			}
		}

		//clearVertexBuffers();
	}

	void OpenGLVertexArray::clearVertexBuffers()
	{
		//empty buffer list after sending data to the gpu now
		mVertexBuffers.clear();
	}

	void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(mRendererId);
		indexBuffer->bind();

		mIndexBuffer = indexBuffer;
	}
}