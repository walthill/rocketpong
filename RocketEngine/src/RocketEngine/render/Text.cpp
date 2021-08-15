#include "Text.h"
#include "RocketEngine/core/RenderCore.h"
#include "RocketEngine/render/Font.h"
#include "platform/OpenGL/OpenGLText.h"

namespace RKTEngine
{
	int Text::sDefaultTextSize = 32;

	Text* Text::create(Font* fontData, std::string& text, int fontSize)
	{
		switch (RenderCore::getAPI())
		{
			case Renderer::API::NONE:		return nullptr;
			case Renderer::API::OPENGL:		return new OpenGLText(fontData, text, fontSize);
		}

		return nullptr;
	}
}