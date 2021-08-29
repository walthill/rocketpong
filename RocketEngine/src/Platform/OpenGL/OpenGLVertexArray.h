#ifndef OPENGL_VERTEX_ARRAY_H
#define OPENGL_VERTEX_ARRAY_H

#include "RocketEngine/render/buffers/VertexArray.h"

namespace RKTEngine
{
	class OpenGLVertexArray : public VertexArray
	{
		public:
			OpenGLVertexArray();
			virtual ~OpenGLVertexArray();

			virtual void bind() const override;
			virtual void unbind() const override;
			virtual uint32 getId() const override { return mRendererId; };

			virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
			virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

			virtual void processVertexBuffers() override;
			virtual void clearVertexBuffers() override;

			virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() override { return mVertexBuffers; }
			virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() override { return mIndexBuffer; }

		private:
			uint32 mRendererId;
			std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffers;
			std::shared_ptr<IndexBuffer> mIndexBuffer;

	};
}

#endif // !OPENGL_VERTEX_ARRAY_H
