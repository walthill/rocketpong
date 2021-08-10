#ifndef OPENGL_TEXT_H
#define OPENGL_TEXT_H

#include <map>
#include "RocketEngine/Defines.h"
#include "RocketEngine/render/Text.h"
#include "RocketEngine/render/buffers/VertexArray.h"
#include "RocketEngine/render/buffers/Texture.h"
#include "RocketEngine/render/shader/Shader.h"
#include <glm/vec2.hpp>
#include <truetype/kc_truetypeassembler.h>

namespace RKTEngine
{	
	class OpenGLText : public Text
	{
		public:
			OpenGLText(std::string fontName);
			~OpenGLText();

			virtual void setText(std::string text) override;

			virtual void renderText() override;
			virtual void renderText(TextData data) override;

			virtual int getWidth() override { return width; };
			virtual int getHeight() override { return height; };

		private:
			const std::string mTEXT_COLOR_UNIFORM = "textColor";
			const std::string mSHADER_ID = "text";
			const std::string mFONT_ASSET_PATH = "assets/fonts/";
			const int defaultFontLoadedSize = 48;
			const int defaultTextDisplaySize = 32;
			
			TTAFont mFontHandle;
			TTAVertexBuffer vb;
			std::shared_ptr<RawTexture> rawTexture;
			std::shared_ptr<VertexArray> glyphVA;
			std::shared_ptr<VertexBuffer> glyphVB;
			std::shared_ptr<IndexBuffer> glyphIB;
			int width, height;
			unsigned char* mFontFileData;

			void initFont(std::string fontName);
			void createFontData();
			unsigned char* loadFont(std::string fontName);
	};
}
#endif // !OPENGL_TEXT_H

