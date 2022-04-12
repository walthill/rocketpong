#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include "RocketEngine/render/buffers/Texture.h"

namespace RKTEngine
{
	class OpenGLTextureHelper
	{
		public:
			static unsigned int setColorChannel(int channels, int type);
			static void setTextureParameters(int sWrapParam = Texture::WrapType::REPEAT, int tWrapParam = Texture::WrapType::REPEAT,
				int miniFilter = Texture::MinifyFilter::MIN_LINEAR, int magFilter = Texture::MagnifyFilter::MAG_LINEAR);

			static const int BORDER_DEFAULT; //"this value must be zero"
	};

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
			virtual void bind(int index) const override;

			virtual bool operator==(const Texture& other) const override { return mRendererId == ((OpenGLTexture2D&)other).mRendererId; }

		private:
			uint32 mWidth, mHeight;
			std::string mPath;
			uint32 mRendererId, mType;
			static const int BORDER_DEFAULT; //"this value must be zero"
	};

	class OpenGLRawTexture : public RawTexture
	{
	public:
		OpenGLRawTexture(void* data, int width, int height, int sWrapParam = WrapType::REPEAT, int tWrapParam = WrapType::REPEAT,
			int miniFilter = MinifyFilter::MIN_LINEAR, int magFilter = MagnifyFilter::MAG_LINEAR, int detailReductionLevel = 0);

		~OpenGLRawTexture();

		virtual uint32 getWidth()		const override { return mWidth; }
		virtual uint32 getHeight()		const override { return mHeight; }
		virtual uint32 getId()			const override { return mRendererId; }
		virtual uint32 getType()		const override { return -1; }
		virtual std::string getPath()	const override { return ""; }

		virtual void bind()			const override;
		virtual void bind(int index) const override;

		virtual bool operator==(const Texture& other) const override { return mRendererId == ((OpenGLRawTexture&)other).mRendererId; }

	private:
		uint32 mWidth, mHeight;
		uint32 mRendererId;
	};

}

#endif // !OPENGL_TEXTURE_H
