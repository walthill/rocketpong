#ifndef FONT_H
#define FONT_H

#include <RKTUtils\Trackable.h>
#include "RocketEngine/Defines.h"

namespace RKTEngine
{
	class Font : public RKTUtil::Trackable
	{
		public:
			virtual ~Font() {};

			static Font* create(unsigned char* fontData);

			virtual void attachFontData() OVERRIDE_REQUIRED;

		protected:
			const int mDEFAULT_TEXT_LOAD_SIZE = 48;
	};
}

#endif // !FONT_H
