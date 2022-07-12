#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include "Component.h"
#include <RocketEngine\render\Color.h>
#include <RocketEngine\render\Text.h>
#include <RocketEngine\render\Font.h>
#include <glm/mat4x4.hpp>

namespace RKTEngine
{
	struct TextData
	{
		Text* mTextInfo;
		std::string mFontName;
		std::string mText;
		Color mColor;
		int mFontSize;

		TextData() : mTextInfo(nullptr), mText("New Text"), mColor(), mFontSize(Text::sDefaultTextSize), mFontName("") {}
		TextData(const std::string& fontName, const std::string& text, int fontSize = Text::sDefaultTextSize, Color color = Color::white) : mFontName(fontName), mTextInfo(nullptr), mText(text), mColor(color), mFontSize(fontSize) {}
	};

	const TextData ZERO_LABEL_DATA;

	class TextComponent : public Component
	{
		public:
			/**********************************************************************//**
			* Creates component with the given id value.
			*
			* @param id ComponentId value assigned to this instance of the Component class
			*************************************************************************/
			TextComponent(const ComponentId& id);

			///Default deconstructor
			~TextComponent();

			void cleanup();
			void load();
			void process(const glm::vec2& position, const glm::vec2& scale, float rotationAngle);
			void render();

			///Access text component data
			inline TextData* getData() { return &mTextData; }

			void setData(const TextData& data);
			
			void setText(const std::string& text);
			void setColor(const Color& color);
			void setFontSize(int fontSize);

		private:
			const std::string mTEXT_COLOR_UNIFORM = "textColor";
			const std::string mSHADER_ID = "text";
			const std::string mMODEL_UNIFORM = "model";
			
			glm::mat4 mModelMatrix;
			TextData mTextData;
	};
}

#endif // !TEXT_COMPONENT_H
