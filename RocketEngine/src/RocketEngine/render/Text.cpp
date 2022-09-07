#include "Text.h"
#include "RocketEngine/render/Renderer.h"
#include "RocketEngine/render/Font.h"
#include "platform/OpenGL/OpenGLText.h"

namespace RKTEngine
{
	int Text::sDefaultTextSize = 28;

	Text* Text::create(Font* fontData, std::string& text, int fontSize)
	{
		switch (Renderer::getAPI())
		{
			case Renderer::API::NONE:		return nullptr;
			case Renderer::API::OPENGL:		return new OpenGLText(fontData, text, fontSize);
		}

		return nullptr;
	}
}