#include "OpenGLRenderer.h"
#include <glad/glad.h>

namespace RKTEngine
{
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
		glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
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
}