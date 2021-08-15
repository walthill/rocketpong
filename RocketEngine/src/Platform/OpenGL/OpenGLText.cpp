#include "OpenGLText.h"
#include "RocketEngine/core/RenderCore.h"
#include "Platform/OpenGL/OpenGLFont.h"

//TODO: word wrapping

namespace RKTEngine
{
	OpenGLText::OpenGLText(Font* fontData, std::string& text, int fontSize)
	{
		mFontData = dynamic_cast<OpenGLFont*>(fontData);
		cursorX = 0;
		cursorY = mSTART_CURSOR_Y;
		initText(text, fontSize);
	}

	OpenGLText::~OpenGLText()
	{
		mFontData = nullptr;
	}

	void OpenGLText::initText(std::string& text, int fontSize)
	{
		setText(text, fontSize);
	}

	void OpenGLText::generateBuffers()
	{
		TTAVertexBuffer vb = kctta_grab_buffer();
		glyphVA.reset(VertexArray::create());

		glyphVB.reset(VertexBuffer::create(vb.vertex_buffer, sizeof(float) * vb.vertices_array_count, VertexBuffer::DataType::STATIC));

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
	}

	void OpenGLText::setText(const std::string& text, int fontSize)
	{
		kctta_clear_buffer();
		kctta_move_cursor(cursorX, mSTART_CURSOR_Y);
		kctta_append_line(text.c_str(), mFontData->getFontData(), fontSize);

		generateBuffers();
	}

	void OpenGLText::process(glm::vec2 position)
	{	
	}

	void OpenGLText::renderText()
	{
		mFontData->attachFontData();
		RenderCore::submit(glyphVA);
	}
}