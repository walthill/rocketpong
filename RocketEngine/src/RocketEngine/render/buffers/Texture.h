#ifndef TEXTURE_H
#define TEXTURE_H

#include <RKTUtils/Trackable.h>
#include "RocketEngine/Defines.h"
#include <vector>

namespace RKTEngine
{
	class Texture : public RKTUtil::Trackable
	{
		public:
			enum WrapType { REPEAT = 0, MIRRORED_REPEAT, CLAMP_EDGE };

			enum MinifyFilter
			{
				MIN_LINEAR = 0, MIN_NEAREST,
				NEAREST_MIPMAP_NEAREST, LINEAR_MIPMAP_NEAREST,
				NEAREST_MIPMAP_LINEAR, LINEAR_MIPMAP_LINEAR
			};

			enum MagnifyFilter { MAG_LINEAR = 0, MAG_NEAREST };

		public:
			virtual ~Texture() {};
			virtual uint32 getWidth()		const OVERRIDE_REQUIRED;
			virtual uint32 getHeight()		const OVERRIDE_REQUIRED;
			virtual uint32 getId()			const OVERRIDE_REQUIRED;
			virtual std::string getPath()	const OVERRIDE_REQUIRED;
			virtual uint32 getType()		const OVERRIDE_REQUIRED;

			virtual void bind()				const OVERRIDE_REQUIRED;

		private:
	};

	class Texture2D : public Texture
	{
		public:
			static Texture2D* create(const std::string& path, int type = 0, int sWrapParam = WrapType::REPEAT, int tWrapParam = WrapType::REPEAT,
				int miniFilter = MinifyFilter::MIN_LINEAR, int magFilter = MagnifyFilter::MAG_LINEAR, int detailReductionLevel = 0);

		private:
	};

	class CubemapTexture : public Texture
	{
		public:
			static CubemapTexture* create(std::vector<std::string> faces, int sWrapParam = WrapType::CLAMP_EDGE, int tWrapParam = WrapType::CLAMP_EDGE,
				int rWrapParam = WrapType::CLAMP_EDGE, int miniFilter = MinifyFilter::MIN_LINEAR,
				int magFilter = MagnifyFilter::MAG_LINEAR, int detailReductionLevel = 0);
		private:
	};
}
#endif // !TEXTURE_H
