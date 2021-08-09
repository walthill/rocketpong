//Public Domain @ https://github.com/kevinmkchin/TrueTypeAssembler

#include "OpenGLText.h"
#include "RocketEngine/core/EngineCore.h"
#include "RocketEngine/core/RenderCore.h"
#include "RocketEngine/render/shader/ShaderManager.h"
#include <truetype/kc_truetypeassembler.cpp>
#include <glad/glad.h>
#include <fstream>
#include <glm\ext\matrix_transform.hpp>

//TODO: update model matrix for position, rotation, scale
//TODO: implement text change at runtime
//TODO: add gameobject abstraction

namespace RKTEngine
{
	OpenGLText::OpenGLText(std::string fontName)
	{
		mTextData.color = Color::white;
		mTextData.position = glm::vec2(20,20); //0,0 is bottom left. for now
		mTextData.scale = 1;
		mTextData.text = "[New] Text";

		const auto& shaderManager = EngineCore::getInstance()->getShaderManager();
		shaderManager->useShaderByKey(mSHADER_ID);
		auto mat = glm::mat4(1.0f);
		auto scale = glm::vec3(1.0f, 1.0f, 1.0f);
		mat = glm::scale(mat, scale); // last scale
		shaderManager->setShaderMat4("model", mat);

		initFont(fontName);
		createFontData();
	}

	OpenGLText::~OpenGLText()
	{
		free(mFontHandle.font_atlas.pixels);
	}

	void OpenGLText::initFont(std::string fontName)
	{
		kctta_setflags(KCTTA_CREATE_INDEX_BUFFER);

		mFontFileData = loadFont(fontName);

		kctta_init_font(&mFontHandle, mFontFileData, defaultFontLoadedSize);
		width = mFontHandle.font_atlas.width;
		height = mFontHandle.font_atlas.height;

		kctta_clear_buffer();
		kctta_move_cursor((int)mTextData.position.x, (int)mTextData.position.y);

		setText(mTextData.text);
	}

	void OpenGLText::createFontData()
	{
		vb = kctta_grab_buffer();

		// Configure VAO/VBO for texture quads
		glyphVA.reset(VertexArray::create());

		glyphVB.reset(VertexBuffer::create(vb.vertex_buffer, sizeof(GLfloat) * vb.vertices_array_count, VertexBuffer::DataType::STATIC));

		BufferLayout layout = {
			{ ShaderDataType::Float4, "vertex" }
		};

		glyphVB->setLayout(layout);
		glyphVA->addVertexBuffer(glyphVB);
		glyphVA->processVertexBuffers();

		glyphIB.reset(IndexBuffer::create(vb.index_buffer, vb.indices_array_count));
		glyphVA->setIndexBuffer(glyphIB);

		glyphVB->unbind();
		glyphVA->unbind();

		// Creating the font texture in GPU memory

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,       // for both the target and source format, we can put GL_RED
			mFontHandle.font_atlas.width,			 // this just means the bit depth is 1 byte (just the alpha)
			mFontHandle.font_atlas.height,
			0, GL_RED, GL_UNSIGNED_BYTE,
			mFontHandle.font_atlas.pixels);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		RenderCommand::setActiveTexture(Renderer::TEX_CHANNEL0);
	}

	void OpenGLText::setText(std::string text)
	{
		kctta_append_line(text.c_str(), &mFontHandle, defaultTextDisplaySize);
		/*kctta_clear_buffer();
		
		createFontData();*/
	}

	void OpenGLText::renderText()
	{
		renderText(mTextData);
	}

	void OpenGLText::renderText(TextData data)
	{
		const auto& shaderManager = EngineCore::getInstance()->getShaderManager();
		shaderManager->useShaderByKey(mSHADER_ID);
		shaderManager->setShaderVec3(mTEXT_COLOR_UNIFORM, data.color.getColor01());

		RenderCommand::setActiveTexture(Renderer::TEX_CHANNEL0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		RenderCore::submit(glyphVA);
	}

	unsigned char* OpenGLText::loadFont(std::string fontName)
	{
		unsigned char* buffer = new unsigned char[1 << 20];
		uint32 bufferSize = 1 << 20;
		if (!fontName.empty())
		{
			auto path = mFONT_ASSET_PATH + fontName;

			//open file

			std::basic_ifstream<unsigned char> infile(path, std::ios::in | std::ifstream::binary);
			
			//get length of file
			infile.seekg(0, std::ios::end);
			auto length = infile.tellg();
			infile.seekg(0, std::ios::beg);

			// don't overflow the buffer!
			if (length > bufferSize)
			{
				length = bufferSize;
			}

			//read file
			infile.read(buffer, length);

			return (unsigned char*)buffer;
		}

		return nullptr;
	}
}