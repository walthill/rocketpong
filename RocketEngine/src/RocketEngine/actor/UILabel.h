#ifndef UI_LABEL_H
#define UI_LABEL_H

#include "Actor.h"
#include "RocketEngine/render/Color.h"

namespace RKTEngine
{
	class UILabel : public Actor
	{
		public:
			UILabel(const glm::vec2& pos, const std::string& text);
			UILabel(const glm::vec2& pos, const std::string& text, int fontSize);
			UILabel(const glm::vec2& pos, const std::string& text, const std::string& fontName, int fontSize);

			virtual void onMessage(Message& message) override;

			std::string getText();
			glm::vec4 getColor();
			std::string getFontName();
			int getFontSize();

			void setText(int text);
			void setText(float text);
			void setText(const std::string& text);
			void setFontSize(int fontSize);
			void setFont(const std::string& fontName);
			void setColor(const Color& color);

		private:

			std::string getTextInternal();
			glm::vec4 getColorInternal();
			std::string getFontNameInternal();
			int getFontSizeInternal();

			void setTextInternal(const std::string& text);
			void setFontSizeInternal(int fontSize);
			void setFontInternal(const std::string& fontName);
			void setColorInternal(const Color& color);
	};
}

#endif // !UI_LABEL_H
