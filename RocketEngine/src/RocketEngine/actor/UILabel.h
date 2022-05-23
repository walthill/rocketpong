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

			virtual void onMessage(RKTEngine::Message& message) override;

			void setText(const std::string& text);
			void setSize(int fontSize);
			void setColor(const Color& color);

		private:
	};
}

#endif // !UI_LABEL_H
