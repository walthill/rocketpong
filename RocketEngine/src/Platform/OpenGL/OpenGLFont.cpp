#include "OpenGLFont.h"
#include "RocketEngine/core/RenderCore.h"

namespace RKTEngine
{
	OpenGLFont::OpenGLFont(unsigned char* fontData)
	{
		init(fontData);
	}

	OpenGLFont::~OpenGLFont()
	{
	}

	void OpenGLFont::init(unsigned char* fontData)
	{
		kctta_setflags(KCTTA_CREATE_INDEX_BUFFER);
		mFontFileData = fontData;
		kctta_init_font(&mFontHandle, fontData, mDEFAULT_TEXT_LOAD_SIZE);

		// Creating the font atlas in GPU memory
		auto fontAtlasData = mFontHandle.font_atlas;
		rawTexture.reset(RawTexture::create(fontAtlasData.pixels, fontAtlasData.width, fontAtlasData.height));
		RenderCommand::setActiveTexture(Renderer::TEX_CHANNEL0);
	}

	void OpenGLFont::attachFontData()
	{
		RenderCommand::setActiveTexture(Renderer::TEX_CHANNEL0);
		rawTexture->bind();
	}

	//For drawing text
	TTAFont* OpenGLFont::getFontData()
	{
		return &mFontHandle;
	}
}