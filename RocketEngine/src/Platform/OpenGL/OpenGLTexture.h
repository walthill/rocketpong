#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include "RocketEngine/render/buffers/Texture.h"

namespace RKTEngine
{
	class OpenGLTexture2D : public Texture2D
	{
		public:
			OpenGLTexture2D(const std::string& path, int type, int sWrapParam = WrapType::REPEAT, int tWrapParam = WrapType::REPEAT,
				int miniFilter = MinifyFilter::MIN_LINEAR, int magFilter = MagnifyFilter::MAG_LINEAR, int detailReductionLevel = 0);

			~OpenGLTexture2D();

			virtual uint32 getWidth()		const override { return mWidth; }
			virtual uint32 getHeight()		const override { return mHeight; }
			virtual uint32 getId()			const override { return mRendererId; }
			virtual uint32 getType()		const override { return mType; }
			virtual std::string getPath()	const override { return mPath; }

			virtual void bind()			const override;

		private:
			uint32 mWidth, mHeight;
			std::string mPath;
			uint32 mRendererId, mType;
			static const int BORDER_DEFAULT; //"this value must be zero"
	};
}

#endif // !OPENGL_TEXTURE_H
