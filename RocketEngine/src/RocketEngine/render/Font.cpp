#include "Font.h"
#include "RocketEngine/core/RenderCore.h"
#include "Platform/OpenGL/OpenGLFont.h"

namespace RKTEngine
{
    Font* Font::create(unsigned char* fontData)
    {
		switch (RenderCore::getAPI())
		{
			case Renderer::API::NONE:		return nullptr;
			case Renderer::API::OPENGL:		return new OpenGLFont(fontData);
		}

        return nullptr;
    }
}