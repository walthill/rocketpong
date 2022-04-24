#ifndef OPENGL_RENDERER_API_H
#define OPENGL_RENDERER_API_H

#include "RocketEngine/render/Renderer.h"

namespace RKTEngine
{
	class OpenGLRenderer : public Renderer
	{
		public:
			virtual void initialize(Shader* renderShader) override;
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
			virtual void setActiveTexture(int offset) override;

			virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
			virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) override;

			
			virtual void drawTriangles(const std::shared_ptr<VertexArray>& vertexArray) override;
			virtual void drawInstancedTriangles(const std::shared_ptr<VertexArray>& vertexArray, int instanceCount) override;
			virtual void drawInstanced(const std::shared_ptr<VertexArray>& vertexArray, int instanceCount) override;

			virtual void drawQuad(const glm::vec2& position, const glm::vec2& size, Texture2D* texture, AtlasCoordinateData atlasCoords, float tilingFactor, const glm::vec4& color) override;
			virtual void drawQuad(const glm::vec3& position, const glm::vec2& size, Texture2D* texture, AtlasCoordinateData atlasCoords, float tilingFactor, const glm::vec4& color) override;
			virtual void drawQuad(const glm::vec2& position, const glm::vec2& size, Texture2D* texture, float tilingFactor, const glm::vec4& color) override;
			virtual void drawQuad(const glm::vec3& position, const glm::vec2& size, Texture2D* texture, float tilingFactor, const glm::vec4& color) override;
			virtual void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) override;

			//rotated color
			virtual void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) override;
			virtual void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) override;
			//rotated texture
			virtual void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Texture2D* texture, float tilingFactor, const glm::vec4& color) override;
			virtual void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Texture2D* texture, float tilingFactor, const glm::vec4& color) override;
			//rotated atlased texture
			virtual void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Texture2D* texture, AtlasCoordinateData atlasData, float tilingFactor, const glm::vec4& color) override;
			virtual void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Texture2D* texture, AtlasCoordinateData atlasData, float tilingFactor, const glm::vec4& color) override;

			virtual Statistics getStats() override;
			virtual void resetStats() override;

		private:
			void FlushAndReset();
			void processQuadData();	//TODO: move quad logic into 1-2 functions
	};
}
#endif // !OPENGL_RENDERER_API_H
