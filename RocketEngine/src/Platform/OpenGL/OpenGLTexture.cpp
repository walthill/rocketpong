#include "OpenGLTexture.h"
#include "RocketEngine/render/Renderer.h"
#include <glad/glad.h>
#include <stb_image.h>


namespace RKTEngine
{

	const int OpenGLTexture2D::BORDER_DEFAULT = 0;

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, int type, int sWrapParam, int tWrapParam, int miniFilter, int magFilter, int detailReductionLevel)
		: mPath(path), mType(type)
	{
		int width, height, channels;

		//1 to flip
		//stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		RKT_ASSERT(data);

		mWidth = width;
		mHeight = height;

		glGenTextures(1, &mRendererId);
		glBindTexture(GL_TEXTURE_2D, mRendererId);

		GLenum format = GL_RGB;
		if (channels == 1)
		{
			format = GL_RED;
		}
		else if (channels == 3)
		{
			if (type == Renderer::TextureType::DIFFUSE)
				format = GL_SRGB;
			else
				format = GL_RGB;
		}
		else if (channels == 4)
		{
			if (type == Renderer::TextureType::DIFFUSE)
				format = GL_SRGB_ALPHA;
			else
				format = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, detailReductionLevel, format, mWidth, mHeight, BORDER_DEFAULT, format, GL_UNSIGNED_BYTE, data);

		//Set texture parameters
		GLint sWrap = -1, tWrap = -1, minificationFilter = -1, magnificationFilter = -1;

		switch (sWrapParam)
		{
			case WrapType::REPEAT:						sWrap = GL_REPEAT;				break;
			case WrapType::MIRRORED_REPEAT:				sWrap = GL_MIRRORED_REPEAT;		break;
			case WrapType::CLAMP_EDGE:					sWrap = GL_CLAMP_TO_EDGE;		break;
		}

		switch (tWrapParam)
		{
			case WrapType::REPEAT:						tWrap = GL_REPEAT;				break;
			case WrapType::MIRRORED_REPEAT:				tWrap = GL_MIRRORED_REPEAT;		break;
			case WrapType::CLAMP_EDGE:					tWrap = GL_CLAMP_TO_EDGE;		break;
		}

		switch (miniFilter)
		{
			case MinifyFilter::MIN_LINEAR:				minificationFilter = GL_LINEAR;				break;
			case MinifyFilter::MIN_NEAREST:				minificationFilter = GL_NEAREST;			break;
			case MinifyFilter::LINEAR_MIPMAP_NEAREST:	minificationFilter = GL_CLAMP_TO_EDGE;		break;
			case MinifyFilter::LINEAR_MIPMAP_LINEAR:	minificationFilter = GL_MIRRORED_REPEAT;	break;
			case MinifyFilter::NEAREST_MIPMAP_NEAREST:	minificationFilter = GL_CLAMP_TO_EDGE;		break;
			case MinifyFilter::NEAREST_MIPMAP_LINEAR:	minificationFilter = GL_MIRRORED_REPEAT;	break;
		}

		switch (magFilter)
		{
			case MagnifyFilter::MAG_LINEAR:				magnificationFilter = GL_LINEAR;		break;
			case MagnifyFilter::MAG_NEAREST:			magnificationFilter = GL_NEAREST;		break;
		}

		//Texture wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
		//Minification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minificationFilter);
		//Magnification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnificationFilter);

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
}