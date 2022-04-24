#include "OpenGLRenderer.h"
#include <glad/glad.h>
#include <RKTUtils/Profiling.h>
#include <glm\ext\matrix_transform.hpp>
#include <RocketEngine/render/shader/Shader.h>

namespace RKTEngine
{
	#pragma region Renderer2DData
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
		static const uint32_t MAX_QUADS = 20000;
		static const uint32_t MAX_VERTICES = MAX_QUADS * 4;
		static const uint32_t MAX_INDICES = MAX_QUADS * 6;
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

		glm::vec4 quadVertexPositions[4];
		Renderer::Statistics stats;

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
#pragma endregion

	void OpenGLRenderer::initialize(Shader* renderShader)
	{
		RKT_PROFILE_FUNCTION();

		sData.quadVertexArray.reset(VertexArray::create());

		sData.quadVertexBuffer.reset(VertexBuffer::create(sData.MAX_VERTICES * sizeof(QuadVertex)));
		sData.quadVertexBuffer->setLayout({
			{ShaderDataType::Float3, "position"},
			{ShaderDataType::Float4, "color"},
			{ShaderDataType::Float2, "texCoords"},
			{ShaderDataType::Float, "texIndex"},
			{ShaderDataType::Float, "tiling"}
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

		uint32_t whiteTextureData = 0xffffff;
		sData.pWhiteTexture = RawTexture::create(&whiteTextureData, 1, 1);
		sData.textureSlots[0] = sData.pWhiteTexture;

		int32_t samplers[sData.MAX_TEXTURE_SLOTS];
		for (size_t i = 0; i < sData.MAX_TEXTURE_SLOTS; i++)
		{
			samplers[i] = i;
		}

		sData.pTextureShader = renderShader;
		sData.pTextureShader->use();
		sData.pTextureShader->setIntArray("textures", samplers, sData.MAX_TEXTURE_SLOTS);
		sData.pTextureShader->setFloat("tiling", 1.0f);

		sData.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		sData.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		sData.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		sData.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void OpenGLRenderer::cleanup()
	{
		RKT_PROFILE_FUNCTION();
		sData.cleanup();
	}

	void OpenGLRenderer::beginScene()
	{
		RKT_PROFILE_FUNCTION();

		sData.pTextureShader->use();

		sData.quadIndexCount = 0;
		sData.textureSlotIndex = 1;
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

		//bind textures
		for (size_t i = 0; i < sData.textureSlotIndex; i++)
		{
			setActiveTexture(i);
			sData.textureSlots[i]->bind();
		}

		drawIndexed(sData.quadVertexArray, sData.quadIndexCount);
#if RKT_RENDER_STATS
		renderStats.drawCalls += 1;
#endif
	}

	void OpenGLRenderer::FlushAndReset()
	{
		endScene();
		sData.quadIndexCount = 0;
		sData.textureSlotIndex = 1;
		sData.quadVertexBufferPtr = sData.quadVertexBufferBase;
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

	void OpenGLRenderer::setActiveTexture(int offset)
	{
		glActiveTexture(GL_TEXTURE0 + offset);
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

	#pragma region Batch Render Calls

	//Atlased Quad
	void OpenGLRenderer::drawQuad(const glm::vec2& position, const glm::vec2& size, Texture2D* texture, AtlasCoordinateData atlasCoords, float tilingFactor, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, atlasCoords, tilingFactor, color);
	}

	void OpenGLRenderer::drawQuad(const glm::vec3& position, const glm::vec2& size, Texture2D* texture, AtlasCoordinateData atlasCoords, float tilingFactor, const glm::vec4& color)
	{
		RKT_PROFILE_FUNCTION();

		//handle batch overflow
		if (sData.quadIndexCount >= Renderer2DData::MAX_INDICES)
			FlushAndReset();

		glm::vec4 defaultColor = Color::white.getColorAlpha01();

		float textureIndex = 0.0f;

		for (size_t i = 1; i < sData.textureSlotIndex; i++)
		{
			if (*sData.textureSlots[i] == *texture)
			{
				//has tex already been submitted
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)sData.textureSlotIndex;
			sData.textureSlots[sData.textureSlotIndex] = texture;
			sData.textureSlotIndex += 1;
		}


		
		/*
			leftX, topY,		//top left					
			 rightX, bottomY,	//bottom right
			 leftX, bottomY,		//bottom left

					//tri #2
			 leftX, topY,		//top left
			 rightX, topY,		//top right
			 rightX, bottomY		//bottom right
		*/

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[0];
		sData.quadVertexBufferPtr->color = defaultColor;
		sData.quadVertexBufferPtr->texCoord = { atlasCoords.leftX, atlasCoords.bottomY };	//0,0 bottom left in OpenGL
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tiling = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[1];
		sData.quadVertexBufferPtr->color = defaultColor;
		sData.quadVertexBufferPtr->texCoord = { atlasCoords.rightX, atlasCoords.bottomY }; //1,0
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tiling = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[2];
		sData.quadVertexBufferPtr->color = defaultColor;
		sData.quadVertexBufferPtr->texCoord = { atlasCoords.rightX, atlasCoords.topY };	//1,1
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tiling = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[3];
		sData.quadVertexBufferPtr->color = defaultColor;
		sData.quadVertexBufferPtr->texCoord = { atlasCoords.leftX, atlasCoords.topY };	//0,1
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tiling = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadIndexCount += 6;

#if RKT_RENDER_STATS
		renderStats.quadCount += 1;
#endif
	}

	//Texture Quad
	void OpenGLRenderer::drawQuad(const glm::vec2& position, const glm::vec2& size, Texture2D* texture, float tilingFactor, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, color);
	}

	void OpenGLRenderer::drawQuad(const glm::vec3& position, const glm::vec2& size, Texture2D* texture, float tilingFactor, const glm::vec4& color)
	{
		RKT_PROFILE_FUNCTION();

		//handle batch overflow
		if (sData.quadIndexCount >= Renderer2DData::MAX_INDICES)
			FlushAndReset();

		glm::vec4 defaultColor = Color::white.getColorAlpha01();

		float textureIndex = 0.0f;

		for (size_t i = 1; i < sData.textureSlotIndex; i++)
		{
			if (*sData.textureSlots[i] == *texture)
			{
				//has tex already been submitted
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)sData.textureSlotIndex;
			sData.textureSlots[sData.textureSlotIndex] = texture;
			sData.textureSlotIndex += 1;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[0];
		sData.quadVertexBufferPtr->color = defaultColor;
		sData.quadVertexBufferPtr->texCoord = { 0.0f,0.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tiling = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[1];
		sData.quadVertexBufferPtr->color = defaultColor;
		sData.quadVertexBufferPtr->texCoord = { 1.0f,0.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tiling = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[2];
		sData.quadVertexBufferPtr->color = defaultColor;
		sData.quadVertexBufferPtr->texCoord = { 1.0f,1.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tiling = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[3];
		sData.quadVertexBufferPtr->color = defaultColor;
		sData.quadVertexBufferPtr->texCoord = { 0.0f,1.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tiling = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadIndexCount += 6;

#if RKT_RENDER_STATS
		renderStats.quadCount += 1;
#endif	
	}

	//Color Quad
	void OpenGLRenderer::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		RKT_PROFILE_FUNCTION();

		//handle batch overflow
		if (sData.quadIndexCount >= Renderer2DData::MAX_INDICES)
			FlushAndReset();

		const float TEX_INDEX = 0.0f;
		const float TILING_FACTOR = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[0];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = {0.0f,0.0f };
		sData.quadVertexBufferPtr->texIndex = TEX_INDEX;	//no texture param -- white texture is always 0
		sData.quadVertexBufferPtr->tiling = TILING_FACTOR;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[1];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 1.0f,0.0f };
		sData.quadVertexBufferPtr->texIndex = TEX_INDEX;
		sData.quadVertexBufferPtr->tiling = TILING_FACTOR;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[2];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 1.0f,1.0f };
		sData.quadVertexBufferPtr->texIndex = TEX_INDEX;
		sData.quadVertexBufferPtr->tiling = TILING_FACTOR;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[3];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 0.0f,1.0f };
		sData.quadVertexBufferPtr->texIndex = TEX_INDEX;
		sData.quadVertexBufferPtr->tiling = TILING_FACTOR;
		sData.quadVertexBufferPtr++;

		sData.quadIndexCount += 6;

#if RKT_RENDER_STATS
		renderStats.quadCount += 1;
#endif
	}

	//rotated color
	void OpenGLRenderer::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		RKT_PROFILE_FUNCTION();

		//handle batch overflow
		if (sData.quadIndexCount >= Renderer2DData::MAX_INDICES)
			FlushAndReset();

		const float TEX_INDEX = 0.0f;
		const float TILING_FACTOR = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[0];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 0.0f,0.0f };
		sData.quadVertexBufferPtr->texIndex = TEX_INDEX;	//no texture param -- white texture is always 0
		sData.quadVertexBufferPtr->tiling = TILING_FACTOR;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[1];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 1.0f,0.0f };
		sData.quadVertexBufferPtr->texIndex = TEX_INDEX;
		sData.quadVertexBufferPtr->tiling = TILING_FACTOR;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[2];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 1.0f,1.0f };
		sData.quadVertexBufferPtr->texIndex = TEX_INDEX;
		sData.quadVertexBufferPtr->tiling = TILING_FACTOR;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[3];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 0.0f,1.0f };
		sData.quadVertexBufferPtr->texIndex = TEX_INDEX;
		sData.quadVertexBufferPtr->tiling = TILING_FACTOR;
		sData.quadVertexBufferPtr++;

		sData.quadIndexCount += 6;

#if RKT_RENDER_STATS
		renderStats.quadCount += 1;
#endif
	}

	//rotated texture
	void OpenGLRenderer::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Texture2D* texture, float tilingFactor, const glm::vec4& color)
	{
		RKT_PROFILE_FUNCTION();

		//handle batch overflow
		if (sData.quadIndexCount >= Renderer2DData::MAX_INDICES)
			FlushAndReset();

		glm::vec4 defaultColor = Color::white.getColorAlpha01();

		float textureIndex = 0.0f;

		for (size_t i = 1; i < sData.textureSlotIndex; i++)
		{
			if (*sData.textureSlots[i] == *texture)
			{
				//has tex already been submitted
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)sData.textureSlotIndex;
			sData.textureSlots[sData.textureSlotIndex] = texture;
			sData.textureSlotIndex += 1;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[0];
		sData.quadVertexBufferPtr->color = defaultColor;
		sData.quadVertexBufferPtr->texCoord = { 0.0f,0.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tiling = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[1];
		sData.quadVertexBufferPtr->color = defaultColor;
		sData.quadVertexBufferPtr->texCoord = { 1.0f,0.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tiling = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[2];
		sData.quadVertexBufferPtr->color = defaultColor;
		sData.quadVertexBufferPtr->texCoord = { 1.0f,1.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tiling = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[3];
		sData.quadVertexBufferPtr->color = defaultColor;
		sData.quadVertexBufferPtr->texCoord = { 0.0f,1.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tiling = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadIndexCount += 6;
			
#if RKT_RENDER_STATS
		renderStats.quadCount += 1;
#endif
	}

	//rotated atlased
	void OpenGLRenderer::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Texture2D* texture, AtlasCoordinateData atlasCoords, float tilingFactor, const glm::vec4& color)
	{
		RKT_PROFILE_FUNCTION();

		//handle batch overflow
		if (sData.quadIndexCount >= Renderer2DData::MAX_INDICES)
			FlushAndReset();

		glm::vec4 defaultColor = Color::white.getColorAlpha01();

		float textureIndex = 0.0f;

		for (size_t i = 1; i < sData.textureSlotIndex; i++)
		{
			if (*sData.textureSlots[i] == *texture)
			{
				//has tex already been submitted
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)sData.textureSlotIndex;
			sData.textureSlots[sData.textureSlotIndex] = texture;
			sData.textureSlotIndex += 1;
		}



		/*
			leftX, topY,		//top left
			 rightX, bottomY,	//bottom right
			 leftX, bottomY,		//bottom left

					//tri #2
			 leftX, topY,		//top left
			 rightX, topY,		//top right
			 rightX, bottomY		//bottom right
		*/

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[0];
		sData.quadVertexBufferPtr->color = defaultColor;
		sData.quadVertexBufferPtr->texCoord = { atlasCoords.leftX, atlasCoords.bottomY };	//0,0 bottom left in OpenGL
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tiling = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[1];
		sData.quadVertexBufferPtr->color = defaultColor;
		sData.quadVertexBufferPtr->texCoord = { atlasCoords.rightX, atlasCoords.bottomY }; //1,0
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tiling = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[2];
		sData.quadVertexBufferPtr->color = defaultColor;
		sData.quadVertexBufferPtr->texCoord = { atlasCoords.rightX, atlasCoords.topY };	//1,1
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tiling = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->position = transform * sData.quadVertexPositions[3];
		sData.quadVertexBufferPtr->color = defaultColor;
		sData.quadVertexBufferPtr->texCoord = { atlasCoords.leftX, atlasCoords.topY };	//0,1
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tiling = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadIndexCount += 6;
#if RKT_RENDER_STATS
		renderStats.quadCount += 1;
#endif
	}
	
	#pragma endregion

	#pragma region Render Stats

	Renderer::Statistics OpenGLRenderer::getStats()
	{
		return renderStats;
	}

	void OpenGLRenderer::resetStats()
	{
		memset(&renderStats, 0, sizeof(Renderer::Statistics));
	}
	#pragma endregion

}