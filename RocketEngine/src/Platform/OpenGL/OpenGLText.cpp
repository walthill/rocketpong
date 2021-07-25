#include "OpenGLText.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>
#include "RocketEngine/core/Log.h"
#include "RocketEngine/core/RenderCore.h"

namespace RKTEngine
{
	OpenGLText::OpenGLText(std::string fontName, const Shader& shader) :
		mpShader(shader)
	{
		mTextData.color = Color::red;
		mTextData.position = glm::vec2(100,100); //0,0 is bottom left. for now
		mTextData.scale = 1;
		mTextData.text = "New Text";

		//mpShader = shader;

		// FreeType
		FT_Library ft;

		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&ft))
		{
			RKT_CORE_ERROR("ERROR::FREETYPE: Could not init FreeType Library");
		}
		// Load font as face
		FT_Face face;
		if (FT_New_Face(ft, (mFONT_ASSET_PATH + fontName).c_str(), 0, &face))
		{
			RKT_CORE_ERROR("ERROR::FREETYPE: Failed to load font");
		}

		// Set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// Disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Load first 128 characters of ASCII set
		for (GLubyte c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				RKT_CORE_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
				continue;
			}
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = {
				texture,
				glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				(uint32)face->glyph->advance.x
			};
			characters.insert(std::pair<GLchar, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		// Destroy FreeType once we're finished
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		// Configure VAO/VBO for texture quads
		glyphVA.reset(VertexArray::create());

		const float* f = {};
		glyphVB.reset(VertexBuffer::create(f, sizeof(GLfloat) * 6 * 4, VertexBuffer::DataType::DYNAMIC));

		BufferLayout layout = {
			{ ShaderDataType::Float4, "vertex" }
		};

		glyphVB->setLayout(layout);
		glyphVA->addVertexBuffer(glyphVB);
		glyphVA->processVertexBuffers();

		uint32 quadIndicies[6] = { 0,1,2,3,4,5 };
		std::shared_ptr<IndexBuffer> glyphIB;
		glyphIB.reset(IndexBuffer::create(quadIndicies, sizeof(quadIndicies) / sizeof(uint32)));
		glyphVA->setIndexBuffer(glyphIB);

		glyphVB->unbind();
		glyphVA->unbind();
	}

	OpenGLText::~OpenGLText()
	{
		//mpShader = nullptr;
	}

	void OpenGLText::renderText()
	{
		renderText(mTextData);
	}

	void OpenGLText::renderText(TextData data)
	{
		// Activate corresponding render state	
		mpShader.use();

		//allow for values from 0 to 255
		mpShader.setVec3(mTEXT_COLOR_UNIFORM, data.color.getColor01());

		RenderCommand::setActiveTexture(Renderer::TextureChannel::TEX_CHANNEL0);
		glyphVA->bind();

		// Iterate through all characters
		std::string::const_iterator c;
		for (c = data.text.begin(); c != data.text.end(); c++)
		{
			Character ch = characters[*c];

			GLfloat xpos = data.position.x + ch.bearing.x * data.scale;
			GLfloat ypos = data.position.y - (ch.size.y - ch.bearing.y) * data.scale;

			GLfloat w = ch.size.x * data.scale;
			GLfloat h = ch.size.y * data.scale;
			// Update VBO for each character
			float vertices[] = {
				 xpos,     ypos + h,   0.0, 0.0 ,
				 xpos,     ypos,       0.0, 1.0 ,
				 xpos + w, ypos,       1.0, 1.0 ,

				 xpos,     ypos + h,   0.0, 0.0 ,
				 xpos + w, ypos,       1.0, 1.0 ,
				 xpos + w, ypos + h,   1.0, 0.0
			};

			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.textureId);

			// Update content of VBO memory
			glyphVB->bind();
			glyphVB->updateBufferData(vertices, sizeof(vertices), 0);
			glyphVB->unbind();

			RenderCore::submit(glyphVA);

			// Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
			float x = (ch.advance >> 6) * data.scale;
			data.position = glm::vec2(data.position.x + x, data.position.y);
		}

		glyphVA->unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}