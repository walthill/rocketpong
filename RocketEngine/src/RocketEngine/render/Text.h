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

			static Text* create(std::string fontName, const Shader& shader);
			void initTextData(TextData data);

			virtual void renderText() OVERRIDE_REQUIRED;
			virtual void renderText(TextData data) OVERRIDE_REQUIRED;

			inline void setText(std::string newText) { mTextData.text = newText; }
			inline std::string getText() { return mTextData.text; }

		protected:
			TextData mTextData;
	};
}
#endif // !TEXT_H
