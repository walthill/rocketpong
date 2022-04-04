#ifndef OPENGL_RENDERER_API_H
#define OPENGL_RENDERER_API_H

#include "RocketEngine/render/Renderer.h"

namespace RKTEngine
{
	class OpenGLRenderer : public Renderer
	{
		public:
			virtual void initialize() override;
			virtual void cleanup() override;

			virtual void beginScene() override;
			virtual void endScene() override;
			virtual void flush() override;

			virtual void clearColor(Color clearColor) override;
			virtual void clearBuffer(int bufferClearFlags) override;
			virtual void setCullOrder(bool isClockwise) override;
			virtual void setCullFace(int faceToCull) override;
			virtual void setDepthBuffer(int depthBufferType) override;
			virtual void setStencilMask(int mask) override;
			virtual void setStencilBuffer(int bufferComparison, int refValue, int mask) override;
			virtual void setActiveTexture(int channel, int offset = 0) override;

			virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
			virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) override;

			
			virtual void drawTriangles(const std::shared_ptr<VertexArray>& vertexArray) override;
			virtual void drawInstancedTriangles(const std::shared_ptr<VertexArray>& vertexArray, int instanceCount) override;
			virtual void drawInstanced(const std::shared_ptr<VertexArray>& vertexArray, int instanceCount) override;

			virtual void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) override;

		private:
	};
}
#endif // !OPENGL_RENDERER_API_H
