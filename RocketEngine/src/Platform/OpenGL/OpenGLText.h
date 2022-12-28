#ifndef OPENGL_TEXT_H
#define OPENGL_TEXT_H

#include "RocketEngine/render/Text.h"
#include "RocketEngine/render/buffers/VertexArray.h"

namespace RKTEngine
{	
	class OpenGLFont;

	class OpenGLText : public Text
	{
		public:
			OpenGLText(Font* fontData, std::string& text, int fontSize);
			~OpenGLText();

			virtual void setText(const std::string& text, int fontSize = Text::sDefaultTextSize) override;

			virtual void process(glm::vec2 position) override;

			virtual void renderText() override;

			virtual int getWidth() override { return width; };
			virtual int getHeight() override { return height; };

		private:
			const int mSTART_CURSOR_Y = 25;
			OpenGLFont* mFontData = nullptr;
			std::shared_ptr<VertexArray> glyphVA = nullptr;
			std::shared_ptr<VertexBuffer> glyphVB = nullptr;
			std::shared_ptr<IndexBuffer> glyphIB = nullptr;
			int width = 0, height = 0;
			int cursorX = 0, cursorY = 0;

			void initText(std::string& text, int fontSize);
			void generateBuffers();
	};
}
#endif // !OPENGL_TEXT_H

