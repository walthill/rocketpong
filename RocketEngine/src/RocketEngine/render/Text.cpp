#include "Text.h"
#include "RocketEngine/core/RenderCore.h"
#include "platform/OpenGL/OpenGLText.h"

namespace RKTEngine
{
	Text* Text::create(std::string fontName/*, const Shader& shader*/)
	{
		switch (RenderCore::getAPI())
		{
			case Renderer::API::NONE:		return nullptr;
			case Renderer::API::OPENGL:		return new OpenGLText(fontName);
		}

		return nullptr;
	}

	void Text::initTextData(TextData data)
	{
		mTextData.text = data.text;
		mTextData.position = data.position;
		mTextData.scale = data.scale;
		mTextData.color = data.color;
	}
}