#ifndef RENDERER_API_H
#define RENDERER_API_H

#include "RocketEngine/render/buffers/VertexArray.h"
#include <RocketEngine/render/buffers/Texture.h>
#include "Color.h"
#include <glm/vec2.hpp>

namespace RKTEngine
{
	class Shader;

	class Renderer
	{
		public:
			enum class API { NONE = 0, OPENGL = 1 };
			enum CullFaceType { BACK = 0, FRONT, FRONT_AND_BACK };
			enum BufferType { COLOR_BUFFER = 1, DEPTH_BUFFER = 2, ACCUM_BUFFER = 4, STENCIL_BUFFER = 8 };
			enum BufferTestType { NEVER = 0, LESS, EQUAL, LESS_OR_EQUAL, GREATER, NOT_EQUAL, GREAT_OR_EQUAL, ALWAYS };
			enum TextureChannel { TEX_CHANNEL0 = 0 };
			enum TextureType { NONE = 0, DIFFUSE, SPECULAR, NORMAL, SPRITE };

			struct Statistics
			{
				uint32_t drawCalls;
				uint32_t quadCount;

				uint32_t getVertexCount() { return quadCount * 4; }
				uint32_t getIndexCount() { return quadCount * 6; }
			} renderStats;

			virtual void initialize(Shader* renderShader) OVERRIDE_REQUIRED;
			virtual void cleanup() OVERRIDE_REQUIRED;

			virtual void beginScene() OVERRIDE_REQUIRED;
			virtual void endScene() OVERRIDE_REQUIRED;
			virtual void flush() OVERRIDE_REQUIRED;

			/***
				* Draw a color to the screen
			****/
			virtual void clearColor(Color clearColor) OVERRIDE_REQUIRED;

			/***
				* Clear OpenGL render buffers based on specified bit parameter
			****/
			virtual void clearBuffer(int bufferClearFlags) OVERRIDE_REQUIRED;

			virtual void setCullOrder(bool isClockwise) OVERRIDE_REQUIRED;

			virtual void setCullFace(int faceToCull) OVERRIDE_REQUIRED;

			//Specifies depth comparison
			virtual void setDepthBuffer(int bufferSettings) OVERRIDE_REQUIRED;

			virtual void setStencilMask(int mask) OVERRIDE_REQUIRED;
			virtual void setStencilBuffer(int bufferComparison, int refValue, int mask) OVERRIDE_REQUIRED;

			virtual void setActiveTexture(int offset) OVERRIDE_REQUIRED;

			virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) OVERRIDE_REQUIRED;
			virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) OVERRIDE_REQUIRED;

			
			virtual void drawTriangles(const std::shared_ptr<VertexArray>& vertexArray) OVERRIDE_REQUIRED;
			virtual void drawInstancedTriangles(const std::shared_ptr<VertexArray>& vertexArray, int instanceCount) OVERRIDE_REQUIRED;
			virtual void drawInstanced(const std::shared_ptr<VertexArray>& vertexArray, int instanceCount) OVERRIDE_REQUIRED;

			//batched draws
			//atlased texture
			virtual void drawQuad(const glm::vec2& position, const glm::vec2& size, Texture2D* texture, AtlasCoordinateData atlasData, float tilingFactor, const glm::vec4& color) OVERRIDE_REQUIRED;
			virtual void drawQuad(const glm::vec3& position, const glm::vec2& size, Texture2D* texture, AtlasCoordinateData atlasData, float tilingFactor, const glm::vec4& color) OVERRIDE_REQUIRED;
			//texture
			virtual void drawQuad(const glm::vec2& position, const glm::vec2& size, Texture2D* texture, float tilingFactor, const glm::vec4& color) OVERRIDE_REQUIRED;
			virtual void drawQuad(const glm::vec3& position, const glm::vec2& size, Texture2D* texture, float tilingFactor, const glm::vec4& color) OVERRIDE_REQUIRED;
			//color
			virtual void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) OVERRIDE_REQUIRED;

			//rotated color
			virtual void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) OVERRIDE_REQUIRED;
			virtual void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) OVERRIDE_REQUIRED;
			//rotated texture
			virtual void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Texture2D* texture, float tilingFactor, const glm::vec4& color) OVERRIDE_REQUIRED;
			virtual void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Texture2D* texture, float tilingFactor, const glm::vec4& color) OVERRIDE_REQUIRED;
			//rotated atlased texture
			virtual void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Texture2D* texture, AtlasCoordinateData atlasData, float tilingFactor, const glm::vec4& color) OVERRIDE_REQUIRED;
			virtual void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Texture2D* texture, AtlasCoordinateData atlasData, float tilingFactor, const glm::vec4& color) OVERRIDE_REQUIRED;

			virtual Statistics getStats() OVERRIDE_REQUIRED;
			virtual void resetStats() OVERRIDE_REQUIRED;

			inline static Renderer::API getAPI() { return msAPI; };
		private:
			static Renderer::API msAPI;
	};
}

#endif // !RENDERER_API_H
