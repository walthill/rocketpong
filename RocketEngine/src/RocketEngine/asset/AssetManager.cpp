#include "AssetManager.h"
#include "RocketEngine/render/buffers/Texture.h"
#include "RocketEngine/render/Font.h"
#include <fstream>

namespace RKTEngine
{
	AssetManager::AssetManager()
	{
	}

	AssetManager::~AssetManager()
	{
		clean();
	}

	bool AssetManager::initialize()
	{
		mSpriteAssetCache = std::map<std::string, Texture2D*>();
		return true;
	}

	void AssetManager::clean()
	{
		auto iter = mSpriteAssetCache.begin();
		for (iter; iter != mSpriteAssetCache.end(); ++iter)
		{
			delete iter->second;
		}

		auto iter2 = mFontAssetCache.begin();
		for (iter2; iter2 != mFontAssetCache.end(); ++iter2)
		{
			delete iter2->second;
		}
	}

	Texture2D* AssetManager::loadSpriteAsset(const std::string& spriteName)
	{
		Texture2D* newSprite = nullptr;
		const auto& iter = mSpriteAssetCache.find(spriteName);
		if (iter != mSpriteAssetCache.end())
		{
			newSprite = iter->second;
		}
		else
		{
			newSprite = Texture2D::create(mSPRITE_ASSET_PATH + spriteName + mSPRITE_FILE_ENDING);
			mSpriteAssetCache[spriteName] = newSprite;
		}

		return newSprite;
	}

	Font* AssetManager::loadFontAsset(const std::string& fontName)
	{
		Font* newFont = nullptr;
		const auto& iter = mFontAssetCache.find(fontName);
		if (iter != mFontAssetCache.end())
		{
			newFont = iter->second;
		}
		else
		{
			auto path = mFONT_ASSET_PATH + fontName;
			newFont = Font::create(loadByteData(path));
			mFontAssetCache[fontName] = newFont;
		}

		return newFont;
	}

	unsigned char* AssetManager::loadByteData(const std::string& path)
	{
		uint32 bufferSize = 1 << 20;
		unsigned char* buffer = new unsigned char[bufferSize];
		std::basic_ifstream<unsigned char> infile(path, std::ios::in | std::ifstream::binary);

		//get length of file
		infile.seekg(0, std::ios::end);
		auto length = infile.tellg();
		infile.seekg(0, std::ios::beg);

		// don't overflow the buffer
		if (length > bufferSize)
		{
			length = bufferSize;
		}

		//read file
		infile.read(buffer, length);

		return buffer;
	}

}