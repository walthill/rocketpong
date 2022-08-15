#ifndef COLOR_H
#define COLOR_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cereal/cereal.hpp>
#include <RKTUtils/Trackable.h>

namespace RKTEngine
{

	class Color : public RKTUtil::Trackable
	{
		public:
			Color();
			Color(float r, float g, float b, float a = 255);
			Color(const glm::vec4& vec4);

			~Color();

			void setColor(glm::vec3 rgb, float a = 255);
			void setColor(glm::vec4 color);
			void setColor(float setAllRGB);
			void setColor(float r, float g, float b, float a = 255);

			inline glm::vec3 getColor() { return glm::vec3(mColorValue.x, mColorValue.y, mColorValue.z); }
			inline glm::vec3 getColor01() { return glm::vec3(getR01(), getG01(), getB01()); }
			inline glm::vec4 getColorAlpha() { return mColorValue; }
			inline glm::vec4 getColorAlpha01() { return glm::vec4(getR01(), getG01(), getB01(), getA01()); }

			//Get individual rgb values
			inline float getR() { return mColorValue.x; }
			inline float getG() { return mColorValue.y; }
			inline float getB() { return mColorValue.z; }
			inline float getA() { return mColorValue.w; }

			//Get rgb values normalized btwn 0,1
			inline float getR01() { return mColorValue.x * RGB01_CONVERSION; }
			inline float getG01() { return mColorValue.y * RGB01_CONVERSION; }
			inline float getB01() { return mColorValue.z * RGB01_CONVERSION; }
			inline float getA01() { return mColorValue.w * RGB01_CONVERSION; }


			static Color black; // (0, 0, 0)
			static Color white; // (255,255,255)
			static Color grey; // (127.5, 127.5,127.5)

			static Color red; // (255,0,0)
			static Color green; // (0,255,0)
			static Color blue; // (0,0,255)

			static Color clear; // (0, 0, 0, 0)

		private:
			glm::vec4 mColorValue;
			static const float RGB01_CONVERSION;

			friend cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("data", mColorValue));
			}

			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("data", mColorValue));
			}
	};
}
#endif // !COLOR_H