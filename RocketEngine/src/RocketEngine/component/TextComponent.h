#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include "Component.h"
#include <RocketEngine\render\Color.h>
#include <RocketEngine\render\Text.h>
#include <RocketEngine\render\Font.h>

namespace RKTEngine
{
	struct TextData : ComponentData
	{
		Text* mTextInfo = nullptr;
		std::string mFontName = "";
		std::string mText = "";
		Color mColor;
		int mFontSize = 0;

		TextData() : ComponentData(), mTextInfo(nullptr), mText("New Text"), mColor(), mFontSize(Text::sDefaultTextSize), mFontName("") {}
		TextData(const std::string& fontName, const std::string& text, int fontSize = Text::sDefaultTextSize, Color color = Color::white, bool enabled = true) 
			: ComponentData(enabled), mFontName(fontName), mTextInfo(nullptr), mText(text), mColor(color), mFontSize(fontSize) {}
	
		template<class Archive>
		void save(Archive& archive) const
		{
			if (this != nullptr)
				archive(CEREAL_NVP(isEnabled), CEREAL_NVP(mFontName), CEREAL_NVP(mText), CEREAL_NVP(mFontSize), MAKE_NVP("Color", mColor));
		}

		template<class Archive>
		void load(Archive& archive)
		{
			try
			{
				archive(CEREAL_NVP(isEnabled), CEREAL_NVP(mFontName), CEREAL_NVP(mText), CEREAL_NVP(mFontSize), MAKE_NVP("Color", mColor));
			}
			catch (cereal::Exception&)
			{
				archive.setNextName(nullptr);
				//help with handling null values here: https://github.com/USCiLab/cereal/issues/30#issuecomment-231848170
				// Loading a key that doesn't exist results in an exception
				// Since "Not Found" is a valid condition for us, we swallow
				// this exception and the archive will not load anything
			}
		}
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
			void setText(int text);
			void setColor(const Color& color);
			void setFontSize(int fontSize);

			inline virtual bool isEnabled() override { return mTextData.isEnabled; }
			inline virtual void setEnabled(bool enabled) override { mTextData.isEnabled = enabled; }

		private:
			const std::string mTEXT_COLOR_UNIFORM = "textColor";
			const std::string mSHADER_ID = "text";
			const std::string mMODEL_UNIFORM = "model";
			
			glm::mat4 mModelMatrix = glm::mat4(1);
			TextData mTextData;
	};
}

#endif // !TEXT_COMPONENT_H
