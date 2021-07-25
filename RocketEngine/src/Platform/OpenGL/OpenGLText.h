#ifndef OPENGL_TEXT_H
#define OPENGL_TEXT_H

#include <map>
#include "RocketEngine/Defines.h"
#include "RocketEngine/render/Text.h"
#include "RocketEngine/render/buffers/VertexArray.h"
#include "RocketEngine/render/shader/Shader.h"
#include <glm/vec2.hpp>

namespace RKTEngine
{	
	class OpenGLText : public Text
	{
		public:
			OpenGLText(std::string fontName, const Shader& shader);
			~OpenGLText();

			virtual void renderText() override;
			virtual void renderText(TextData data) override;

		private:
			struct Character
			{
				uint32 textureId;
				glm::vec2 size;
				glm::vec2 bearing;
				uint32 advance;
			};

			const std::string mTEXT_COLOR_UNIFORM = "textColor";
			const std::string mFONT_ASSET_PATH = "assets/fonts/";

			const Shader& mpShader;
			std::shared_ptr<VertexBuffer> glyphVB;
			std::shared_ptr<VertexArray> glyphVA;

			std::map<char, Character> characters;
	};
}
#endif // !OPENGL_TEXT_H

