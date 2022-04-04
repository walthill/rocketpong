#ifndef OPENGL_BUFFER_H
#define OPENGL_BUFFER_H

#include "RocketEngine/render/buffers/Buffer.h"

namespace RKTEngine
{
	//Vertex Buffer - OpenGL VBO
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32 size, DataType usage);	//for initializing empty, dynamic batch buffer
		OpenGLVertexBuffer(const float* vertices, uint32 size, DataType usage);
		OpenGLVertexBuffer(const void* vertices, uint32 size, DataType usage);
		virtual ~OpenGLVertexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void setRenderData(const void* data, uint32_t size) override;

		virtual void setLayout(const BufferLayout& layout) override { mLayout = layout; }
		virtual const BufferLayout& getLayout() const override { return mLayout; }

		//Used to update some portion of an existing buffer (call glBufferSubData)
		virtual void updateBufferData(float* vertices, uint32 size, int offset) override;

	private:
		uint32 mRendererId = -1;
		BufferLayout mLayout;
	};

	//Index Buffer - OpenGL EBO
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32* indices, uint32 count);
		virtual ~OpenGLIndexBuffer();

		virtual void bind() const;
		virtual void unbind() const;

		virtual uint32 getCount() const { return mCount; };
	private:
		uint32 mRendererId = -1;
		uint32 mCount;
	};

	//Frame Buffer - OpenGL FBO
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(int texWidth, int texHeight, int aaSamples = 1);
		virtual ~OpenGLFrameBuffer();

		virtual void bind() const;
		virtual void bindScreenTexture() const;
		virtual void unbind() const;
		virtual void blit() const;

		virtual uint32 getTexture() const { return mScreenTexture; };
	private:
		int mTexWidth, mTexHeight;		// move to texture class
		uint32 mTextureColorBuffer, mScreenTexture;		//TODO: texture class
		uint32 mIntermediateFB;
		uint32 mFramebufferId = -1;
	};
}

#endif // !OPENGL_BUFFER_H
