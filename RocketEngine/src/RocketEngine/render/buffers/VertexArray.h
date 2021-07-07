#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "Buffer.h"

namespace RKTEngine
{
	//TODO: track instance count inside vertex array???
	class VertexArray : public RKTUtil::Trackable
	{
		public:
			virtual ~VertexArray() {}

			virtual void bind() const OVERRIDE_REQUIRED;
			virtual void unbind() const OVERRIDE_REQUIRED;
			virtual uint32 getId() const OVERRIDE_REQUIRED;
			virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) OVERRIDE_REQUIRED;
			virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) OVERRIDE_REQUIRED;

			virtual void processVertexBuffers() OVERRIDE_REQUIRED;

			virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() OVERRIDE_REQUIRED;
			virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() OVERRIDE_REQUIRED;

			static VertexArray* create();

		private:
	};
}

#endif // !VERTEX_ARRAY_H
