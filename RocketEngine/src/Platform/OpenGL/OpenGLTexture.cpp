#include "OpenGLTexture.h"
#include "RocketEngine/render/Renderer.h"
#include <stb_image.h>
#include <glad/glad.h>

namespace RKTEngine
{
	/* ~~~ TEXTURE 2D ~~~ */
	#pragma region TEXTURE2D

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, int type, int sWrapParam, int tWrapParam, int miniFilter, int magFilter, int detailReductionLevel)
		: mPath(path), mType(type)
	{
		int width, height, channels;

		//1 to flip
		//stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data == nullptr)
			RKT_CORE_ERROR(stbi_failure_reason());

		RKT_ASSERT(data);

		mWidth = width;
		mHeight = height;

		glGenTextures(1, &mRendererId);
		bind();

		GLenum format = OpenGLTextureHelper::setColorChannel(channels, type);

		glTexImage2D(GL_TEXTURE_2D, detailReductionLevel, format, mWidth, mHeight, OpenGLTextureHelper::BORDER_DEFAULT, format, GL_UNSIGNED_BYTE, data);

		OpenGLTextureHelper::setTextureParameters(sWrapParam, tWrapParam, miniFilter, magFilter);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		//  NOTE: Freed when the OpenGL context is cleaned up. 
		// Not explicitly deleting here to avoid requiring all Texture2D objects to be stored in program memory
		//glDeleteTextures(1, &mRendererId); 
	}

	void OpenGLTexture2D::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, mRendererId);
	}

	void OpenGLTexture2D::bind(int index) const
	{
		glBindTexture(GL_TEXTURE_2D, index);
	}

	#pragma endregion

	/* ~~~ RAW TEXTURE ~~~ */
	#pragma region RAW TEXTURE

	OpenGLRawTexture::OpenGLRawTexture(void* data, int width, int height, int sWrapParam, int tWrapParam ,
		int miniFilter, int magFilter, int detailReductionLevel)
	{
		RKT_ASSERT(data);

		mWidth = width;
		mHeight = height;

		glGenTextures(1, &mRendererId);
		bind();

		GLenum format = OpenGLTextureHelper::setColorChannel(ColorChannel::RGB_ALPHA, Renderer::TextureType::NONE);
		//GL_RGB8 - 8 bits per color value
		glTexImage2D(GL_TEXTURE_2D, detailReductionLevel, GL_RGB8, width, height, OpenGLTextureHelper::BORDER_DEFAULT, format, GL_UNSIGNED_BYTE, data);

		OpenGLTextureHelper::setTextureParameters(sWrapParam, tWrapParam, miniFilter, magFilter);
		glGenerateMipmap(GL_TEXTURE_2D);
	}


	OpenGLRawTexture::~OpenGLRawTexture()
	{
		//  NOTE: Freed when the OpenGL context is cleaned up. 
		// Not explicitly deleting here to avoid requiring all Texture2D objects to be stored in program memory
		//glDeleteTextures(1, &mRendererId); 
	}

	void OpenGLRawTexture::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, mRendererId);
	}

	void OpenGLRawTexture::bind(int index) const
	{
	}

	#pragma endregion

	/* ~~~ HELPERS ~~~ */
	#pragma region HELPERS
	
	const int OpenGLTextureHelper::BORDER_DEFAULT = 0;

	uint32 OpenGLTextureHelper::setColorChannel(int channels, int type)
	{
		uint32 format = GL_RGB;

		if (channels == Texture::ColorChannel::RED)
		{
			format = GL_RED;
		}
		else if (channels == Texture::ColorChannel::RGB)
		{
			if (type == Renderer::TextureType::DIFFUSE)
				format = GL_SRGB;
			else
				format = GL_RGB;
		}
		else if (channels == Texture::ColorChannel::RGB_ALPHA)
		{
			if (type == Renderer::TextureType::DIFFUSE)
				format = GL_SRGB_ALPHA;
			else
				format = GL_RGBA;
		}

		return format;
	}

	void OpenGLTextureHelper::setTextureParameters(int sWrapParam, int tWrapParam, int miniFilter, int magFilter)
	{
		//Set texture parameters
		GLint sWrap = -1, tWrap = -1, minificationFilter = -1, magnificationFilter = -1;

		switch (sWrapParam)
		{
		case Texture::WrapType::REPEAT:						sWrap = GL_REPEAT;				break;
		case Texture::WrapType::MIRRORED_REPEAT:			sWrap = GL_MIRRORED_REPEAT;		break;
		case Texture::WrapType::CLAMP_EDGE:					sWrap = GL_CLAMP_TO_EDGE;		break;
		}

		switch (tWrapParam)
		{
		case Texture::WrapType::REPEAT:						tWrap = GL_REPEAT;				break;
		case Texture::WrapType::MIRRORED_REPEAT:			tWrap = GL_MIRRORED_REPEAT;		break;
		case Texture::WrapType::CLAMP_EDGE:					tWrap = GL_CLAMP_TO_EDGE;		break;
		}

		switch (miniFilter)
		{
		case Texture::MinifyFilter::MIN_LINEAR:				minificationFilter = GL_LINEAR;				break;
		case Texture::MinifyFilter::MIN_NEAREST:			minificationFilter = GL_NEAREST;			break;
		case Texture::MinifyFilter::LINEAR_MIPMAP_NEAREST:	minificationFilter = GL_CLAMP_TO_EDGE;		break;
		case Texture::MinifyFilter::LINEAR_MIPMAP_LINEAR:	minificationFilter = GL_MIRRORED_REPEAT;	break;
		case Texture::MinifyFilter::NEAREST_MIPMAP_NEAREST:	minificationFilter = GL_CLAMP_TO_EDGE;		break;
		case Texture::MinifyFilter::NEAREST_MIPMAP_LINEAR:	minificationFilter = GL_MIRRORED_REPEAT;	break;
		}

		switch (magFilter)
		{
		case Texture::MagnifyFilter::MAG_LINEAR:			magnificationFilter = GL_LINEAR;		break;
		case Texture::MagnifyFilter::MAG_NEAREST:			magnificationFilter = GL_NEAREST;		break;
		}

		//Texture wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
		//Minification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minificationFilter);
		//Magnification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnificationFilter);
	}

	#pragma endregion


}