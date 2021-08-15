#ifndef TEXT_H
#define TEXT_H

#include <glm/vec2.hpp>
#include <RKTUtils/Trackable.h>
#include "RocketEngine/Defines.h"
#include "Color.h"

namespace RKTEngine
{
	class Font;

	class Text : public RKTUtil::Trackable
	{
		public:
			virtual ~Text() {};

			static Text* create(Font* fontData, std::string& text, int fontSize);

			virtual void setText(const std::string& newText, int fontSize = sDefaultTextSize) OVERRIDE_REQUIRED;

			virtual void process(glm::vec2 position) OVERRIDE_REQUIRED;

			virtual void renderText() OVERRIDE_REQUIRED;

			virtual int getWidth() OVERRIDE_REQUIRED;
			virtual int getHeight() OVERRIDE_REQUIRED;
		
			static int sDefaultTextSize;
		protected:
	};
}
#endif // !TEXT_H
