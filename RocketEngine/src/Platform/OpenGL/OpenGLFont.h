#ifndef OPENGL_FONT_H
#define OPENGL_FONT_H

#include "RocketEngine/render/Font.h"
#include "RocketEngine/render/buffers/Texture.h"
#include <kc_truetypeassembler.h>

namespace RKTEngine
{
	class OpenGLFont : public Font
	{
	public:
		OpenGLFont(unsigned char* fontData);
		~OpenGLFont();

		void init(unsigned char* fontData);

		virtual void attachFontData() override;

		TTAFont* getFontData();

	private:
		TTAFont mFontHandle;
		std::shared_ptr<RawTexture> rawTexture;
		unsigned char* mFontFileData = nullptr;
	};
}
#endif // !OPENGL_TEXT_H

