#include "OpenGLBuffer.h"
#include <glad/glad.h>

namespace RKTEngine
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, uint32 size, DataType usage) //size in bytes
	{
		glGenBuffers(1, &mRendererId);
		glBindBuffer(GL_ARRAY_BUFFER, mRendererId);

		switch (usage)
		{
			case VertexBuffer::DataType::STATIC:
				glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
				break;
			case VertexBuffer::DataType::DYNAMIC:
				glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
				break;
			case VertexBuffer::DataType::STREAM:
				glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STREAM_DRAW);
				break;
		}
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* vertices, uint32 size, DataType usage) //size in bytes
	{
		glGenBuffers(1, &mRendererId);
		glBindBuffer(GL_ARRAY_BUFFER, mRendererId);

		switch (usage)
		{
			case VertexBuffer::DataType::STATIC:
				glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
				break;
			case VertexBuffer::DataType::DYNAMIC:
				glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
				break;
			case VertexBuffer::DataType::STREAM:
				glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STREAM_DRAW);
				break;
		}
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &mRendererId);
	}

	void OpenGLVertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, mRendererId);
	}

	void OpenGLVertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::updateBufferData(float* vertices, uint32 size, int offset)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
	}

	/*********************************************************************

	Index Buffer for OpenGL -- aka Element Buffer Object (EBO)

	**********************************************************************/

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32* indices, uint32 count) : //count = number of elements
		mCount(count)
	{
		glGenBuffers(1, &mRendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &mRendererId);
	}

	void OpenGLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererId);
	}

	void OpenGLIndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	/*********************************************************************

	Frame Buffer for OpenGL -- aka Frame Buffer Object (FBO)

	**********************************************************************/

	OpenGLFrameBuffer::OpenGLFrameBuffer(int texWidth, int texHeight, int aaSamples) :
		mTexWidth(texWidth),
		mTexHeight(texHeight)
	{
		//TODO: pass in format data as parameter
		//Make struct to hold object data??

		// configure MSAA framebuffer
		// --------------------------
		glGenFramebuffers(1, &mFramebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferId);
		// create a multisampled color attachment texture
		glGenTextures(1, &mTextureColorBuffer);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mTextureColorBuffer);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, aaSamples, GL_RGB, mTexWidth, mTexHeight, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, mTextureColorBuffer, 0);
		// create a (also multisampled) renderbuffer object for depth and stencil attachments
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, aaSamples, GL_DEPTH24_STENCIL8, mTexWidth, mTexHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			//RKT_ASSERT("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// configure second post-processing framebuffer
		glGenFramebuffers(1, &mIntermediateFB);
		glBindFramebuffer(GL_FRAMEBUFFER, mIntermediateFB);
		// create a color attachment texture
		glGenTextures(1, &mScreenTexture);
		glBindTexture(GL_TEXTURE_2D, mScreenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mTexWidth, mTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mScreenTexture, 0);	// we only need a color buffer

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			//RKT_ASSERT("ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &mFramebufferId);
	}

	void OpenGLFrameBuffer::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferId);
	}

	void OpenGLFrameBuffer::bindScreenTexture() const
	{
		glBindTexture(GL_TEXTURE_2D, mScreenTexture);
	}

	void OpenGLFrameBuffer::blit() const
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, mFramebufferId);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mIntermediateFB);
		glBlitFramebuffer(0, 0, mTexWidth, mTexHeight, 0, 0, mTexWidth, mTexHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	void OpenGLFrameBuffer::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
