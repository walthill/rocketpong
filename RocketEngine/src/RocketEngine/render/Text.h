#ifndef TEXT_H
#define TEXT_H

#include <glm/vec2.hpp>
#include <RKTUtils/Trackable.h>
#include "RocketEngine/Defines.h"
#include "Color.h"

namespace RKTEngine
{

	struct TextData
	{
		std::string text;
		Color color;
		glm::vec2 position;
		float scale;
	};

	class Shader;

	class Text : public RKTUtil::Trackable
	{
		public:
			virtual ~Text() {};

			static Text* create(std::string fontName/*, const Shader& shader*/);
			void initTextData(TextData data);

			virtual void setText(std::string newText) OVERRIDE_REQUIRED;

			virtual void renderText() OVERRIDE_REQUIRED;
			virtual void renderText(TextData data) OVERRIDE_REQUIRED;

			virtual int getWidth() OVERRIDE_REQUIRED;
			virtual int getHeight() OVERRIDE_REQUIRED;

			inline std::string getText() { return mTextData.text; }

		protected:
			TextData mTextData;
	};
}
#endif // !TEXT_H
