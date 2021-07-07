#include "Color.h"

namespace RKTEngine
{
	const float Color::RGB01_CONVERSION = 0.00392156863f;

	Color Color::black(0, 0, 0);
	Color Color::white(255, 255, 255);
	Color Color::grey(127.5f, 127.5f, 127.5f);

	Color Color::red(255, 0, 0);
	Color Color::green(0, 255, 0);
	Color Color::blue(0, 0, 255);

	Color Color::clear(0, 0, 0, 0);

	Color::Color()
	{
		mColorValue = glm::vec4(0, 0, 0, 255);
	}


	Color::Color(float r, float g, float b, float a)
	{
		mColorValue = glm::vec4(r, g, b, a);
	}


	Color::~Color()
	{
	}

	void Color::setColor(glm::vec3 rgb, float a)
	{
		mColorValue = glm::vec4(rgb, a);
	}

	void Color::setColor(glm::vec4 color)
	{
		mColorValue = color;
	}


	void Color::setColor(float r, float g, float b, float a)
	{
		mColorValue = glm::vec4(r, g, b, a);
	}


	void Color::setColor(float setRGB)
	{
		mColorValue = glm::vec4(setRGB, setRGB, setRGB, setRGB);
	}
}