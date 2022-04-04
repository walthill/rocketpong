#include "OpenGLRenderer.h"
#include <glad/glad.h>
#include <RKTUtils/Profiling.h>

namespace RKTEngine
{
	void OpenGLRenderer::initialize()
	{
		RKT_PROFILE_FUNCTION();

		sData.quadVertexArray.reset(VertexArray::create());

		sData.quadVertexBuffer.reset(VertexBuffer::create(sData.MAX_VERTICES * sizeof(QuadVertex)));
		sData.quadVertexBuffer->setLayout({
			{ShaderDataType::Float3, "position"},
			{ShaderDataType::Float4, "a_color"},
			{ShaderDataType::Float2, "texCoords"}
		});
		sData.quadVertexArray->addVertexBuffer(sData.quadVertexBuffer);
		sData.quadVertexArray->processVertexBuffers();

		sData.quadVertexBufferBase = new QuadVertex[sData.MAX_VERTICES];

		uint32_t* quadIndices = new uint32_t[sData.MAX_INDICES];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < sData.MAX_INDICES; i+=6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		std::shared_ptr<IndexBuffer> quadIB;
		quadIB.reset(IndexBuffer::create(quadIndices, sData.MAX_INDICES));
		sData.quadVertexArray->setIndexBuffer(quadIB);
		delete[] quadIndices;
	}

	void OpenGLRenderer::cleanup()
	{
		RKT_PROFILE_FUNCTION();
		sData.cleanup();
	}

	void OpenGLRenderer::beginScene()
	{
		RKT_PROFILE_FUNCTION();

		sData.quadIndexCount = 0;
		sData.quadVertexBufferPtr = sData.quadVertexBufferBase;
	}

	void OpenGLRenderer::endScene()
	{
		RKT_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)sData.quadVertexBufferPtr - (uint8_t*)sData.quadVertexBufferBase;
		sData.quadVertexBuffer->setRenderData(sData.quadVertexBufferBase, dataSize);
		flush();
	}

	void OpenGLRenderer::flush()
	{
		RKT_PROFILE_FUNCTION();

		drawIndexed(sData.quadVertexArray, sData.quadIndexCount);
	}

	void OpenGLRenderer::clearColor(Color clearColor)
	{
		glClearColor(clearColor.getR01(), clearColor.getG01(), clearColor.getB01(), clearColor.getA01());
	}

	void OpenGLRenderer::clearBuffer(int bufferClearFlags)
	{
		if (bufferClearFlags & COLOR_BUFFER)
			glClear(GL_COLOR_BUFFER_BIT);
		if (bufferClearFlags & DEPTH_BUFFER)
			glClear(GL_DEPTH_BUFFER_BIT);
		/*if (buffersToClear & ACCUM_BUFFER)	//Seems to be deprecated in current glfw/OpenGL
			glClear(GL_ACCUM_BUFFER);*/			// http://stackoverflow.com/questions/23990366/ddg#23995804
		if (bufferClearFlags & STENCIL_BUFFER)
			glClear(GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLRenderer::setCullOrder(bool isClockwise)
	{
		isClockwise ? glFrontFace(GL_CW) : glFrontFace(GL_CCW);
	}


	void OpenGLRenderer::setCullFace(int faceToCull)
	{
		GLenum cullFaceType{};
		switch (faceToCull)
		{
			case BACK:					cullFaceType = GL_BACK;				break;
			case FRONT:					cullFaceType = GL_FRONT;			break;
			case FRONT_AND_BACK:		cullFaceType = GL_FRONT_AND_BACK;	break;
		}

		glCullFace(cullFaceType);
	}

	void OpenGLRenderer::setDepthBuffer(int depthBufferType)
	{
		GLenum depthComparisonType{};
		switch (depthBufferType)
		{
			case NEVER:				depthComparisonType = GL_NEVER;		break;
			case LESS:				depthComparisonType = GL_LESS;		break;
			case LESS_OR_EQUAL:		depthComparisonType = GL_LEQUAL;	break;
			case EQUAL:				depthComparisonType = GL_EQUAL;		break;
			case NOT_EQUAL:			depthComparisonType = GL_NOTEQUAL;	break;
			case GREATER:			depthComparisonType = GL_GREATER;	break;
			case GREAT_OR_EQUAL:	depthComparisonType = GL_GEQUAL;	break;
			case ALWAYS:			depthComparisonType = GL_ALWAYS;	break;
		}

		glDepthFunc(depthComparisonType);
	}

	void OpenGLRenderer::setStencilMask(int mask)
	{
		//0x00 disables writing to the stencil buffer
		//0xFF enables writing to the stencil buffer
		glStencilMask(mask);
	}

	void OpenGLRenderer::setStencilBuffer(int bufferComparison, int refValue, int mask)
	{
		//ref value should be 0 or 1
		GLenum stencilComparisonType{};
		switch (bufferComparison)
		{
			case NEVER:				stencilComparisonType = GL_NEVER;		break;
			case LESS:				stencilComparisonType = GL_LESS;		break;
			case LESS_OR_EQUAL:		stencilComparisonType = GL_LEQUAL;		break;
			case EQUAL:				stencilComparisonType = GL_EQUAL;		break;
			case NOT_EQUAL:			stencilComparisonType = GL_NOTEQUAL;	break;
			case GREATER:			stencilComparisonType = GL_GREATER;		break;
			case GREAT_OR_EQUAL:	stencilComparisonType = GL_GEQUAL;		break;
			case ALWAYS:			stencilComparisonType = GL_ALWAYS;		break;
		}
		glStencilFunc(stencilComparisonType, refValue, mask);
	}

	void OpenGLRenderer::setActiveTexture(int channel, int offset)
	{
		GLenum texChannel{};
		switch (channel)
		{
			case TEX_CHANNEL0:		texChannel = GL_TEXTURE0 + offset;	break;
		}

		glActiveTexture(texChannel);
	}

	void OpenGLRenderer::drawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->bind();
		glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderer::drawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->bind();
		uint32_t count = indexCount == 0 ? vertexArray->getIndexBuffer()->getCount() : indexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderer::drawTriangles(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void OpenGLRenderer::drawInstancedTriangles(const std::shared_ptr<VertexArray>& vertexArray, int instanceCount)
	{
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instanceCount);
	}

	void OpenGLRenderer::drawInstanced(const std::shared_ptr<VertexArray>& vertexArray, int instanceCount)
	{
		glDrawElementsInstanced(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, 0, instanceCount);
	}

	void OpenGLRenderer::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		RKT_PROFILE_FUNCTION();

		sData.quadVertexBufferPtr->position = position;
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = {0.0f,0.0f };
		sData.quadVertexBufferPtr++;


		sData.quadVertexBufferPtr->position = { position.x + size.x, position.y, 0.0f };
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 1.0f,0.0f };
		sData.quadVertexBufferPtr++;


		sData.quadVertexBufferPtr->position = { position.x + size.x, position.y + size.y, 0.0f };
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 1.0f,1.0f };
		sData.quadVertexBufferPtr++;



		sData.quadVertexBufferPtr->position = { position.x, position.y + size.y, 0.0f };
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 0.0f,1.0f };
		sData.quadVertexBufferPtr++;

		sData.quadIndexCount += 6;

	}
}