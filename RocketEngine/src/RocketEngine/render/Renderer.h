#ifndef RENDERER_API_H
#define RENDERER_API_H

#include "RocketEngine/render/buffers/VertexArray.h"
#include <RocketEngine/render/buffers/Texture.h>
#include "Color.h"
#include <glm/vec2.hpp>
#include <RocketEngine\render\shader\Shader.h>

namespace RKTEngine
{


	class Renderer
	{
		public:
			//Order matters here for uploading and parsing bytes on the shader
			struct QuadVertex
			{
				glm::vec3 position;
				glm::vec4 color;
				glm::vec2 texCoord;
				float texIndex;
				float tiling;
			};

			struct Renderer2DData
			{
				const uint32_t MAX_QUADS = 10000;
				const uint32_t MAX_VERTICES = MAX_QUADS * 4;
				const uint32_t MAX_INDICES = MAX_QUADS * 6;
				static const uint32_t MAX_TEXTURE_SLOTS = 32;

				uint32_t quadIndexCount = 0;
				std::shared_ptr<VertexArray> quadVertexArray;
				std::shared_ptr<VertexBuffer> quadVertexBuffer;
				QuadVertex* quadVertexBufferBase = nullptr;
				QuadVertex* quadVertexBufferPtr = nullptr;

				Shader* pTextureShader;
				RawTexture* pWhiteTexture;

				//hold all texture slots
				std::array<Texture2D*, MAX_TEXTURE_SLOTS> textureSlots;
				uint32_t textureSlotIndex = 1; //0 = white textures in tutorial

				void cleanup()
				{
					quadVertexArray.reset();
					quadVertexBuffer.reset();
					delete pWhiteTexture;
					pWhiteTexture = nullptr;
					if (pTextureShader != nullptr)
						pTextureShader = nullptr;
				}
			};
			static Renderer2DData sData;

			enum class API { NONE = 0, OPENGL = 1 };
			enum CullFaceType { BACK = 0, FRONT, FRONT_AND_BACK };
			enum BufferType { COLOR_BUFFER = 1, DEPTH_BUFFER = 2, ACCUM_BUFFER = 4, STENCIL_BUFFER = 8 };
			enum BufferTestType { NEVER = 0, LESS, EQUAL, LESS_OR_EQUAL, GREATER, NOT_EQUAL, GREAT_OR_EQUAL, ALWAYS };
			enum TextureChannel { TEX_CHANNEL0 = 0 };
			enum TextureType { NONE = 0, DIFFUSE, SPECULAR, NORMAL, SPRITE };
			
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
			
			virtual void drawQuad(const glm::vec2& position, const glm::vec2& size, Texture2D* texture, AtlasCoordinateData atlasData, float tilingFactor, const glm::vec4& color) OVERRIDE_REQUIRED;
			virtual void drawQuad(const glm::vec3& position, const glm::vec2& size, Texture2D* texture, AtlasCoordinateData atlasData, float tilingFactor, const glm::vec4& color) OVERRIDE_REQUIRED;

			virtual void drawQuad(const glm::vec2& position, const glm::vec2& size, Texture2D* texture, float tilingFactor, const glm::vec4& color) OVERRIDE_REQUIRED;
			virtual void drawQuad(const glm::vec3& position, const glm::vec2& size, Texture2D* texture, float tilingFactor, const glm::vec4& color) OVERRIDE_REQUIRED;

			virtual void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) OVERRIDE_REQUIRED;

			inline static Renderer::API getAPI() { return msAPI; };

		private:
			static Renderer::API msAPI;
	};
}

#endif // !RENDERER_API_H
