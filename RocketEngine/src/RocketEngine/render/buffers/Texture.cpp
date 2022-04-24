#include "Texture.h"
#include "RocketEngine/render/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
//#include "Platform/OpenGL/OpenGLCubemap.h"

namespace RKTEngine
{
	Texture2D* Texture2D::create(const std::string& path, int type, int sWrapParam, int tWrapParam, int miniFilter, int magFilter, int detailReductionLevel)
	{
		switch (Renderer::getAPI())
		{
			case Renderer::API::NONE:		return nullptr;
			case Renderer::API::OPENGL:		return new OpenGLTexture2D(path, type, sWrapParam, tWrapParam, miniFilter, magFilter, detailReductionLevel);
		}

		return nullptr;
	}

	RawTexture* RawTexture::create(void* data, int width, int height, int sWrapParam, int tWrapParam, int miniFilter, int magFilter, int detailReductionLevel)
	{
		switch (Renderer::getAPI())
		{
			case Renderer::API::NONE:		return nullptr;
			case Renderer::API::OPENGL:		return new OpenGLRawTexture(data, width, height, sWrapParam, tWrapParam, miniFilter, magFilter, detailReductionLevel);
		}

		return nullptr;
	}

	/*CubemapTexture* CubemapTexture::create(std::vector<std::string> faces, int sWrapParam, int tWrapParam, int rWrapParam,
										  int miniFilter, int magFilter, int detailReductionLevel)
	{
		switch (Renderer::getAPI())
		{
			case Renderer::API::NONE:		return nullptr;
			case Renderer::API::OPENGL:		return new OpenGLCubemap(faces, sWrapParam, tWrapParam, rWrapParam, miniFilter, magFilter, detailReductionLevel);
		}

		return nullptr;
	}*/
}