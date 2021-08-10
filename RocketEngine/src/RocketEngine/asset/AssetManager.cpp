#include "AssetManager.h"
#include "RocketEngine/render/buffers/Texture.h"


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
	}

	Texture2D* AssetManager::loadSpriteAsset(std::string spriteName)
	{
		const auto& iter = mSpriteAssetCache.find(spriteName);
		if (iter != mSpriteAssetCache.end())
		{
			return iter->second;
		}
		else
		{
			Texture2D* newSprite = Texture2D::create(mSPRITE_ASSET_PATH + spriteName + mSPRITE_FILE_ENDING);
			mSpriteAssetCache[spriteName] = newSprite;

			return newSprite;
		}
	}
	unsigned char* AssetManager::loadFont(std::string fontName)
	{
		return nullptr;
	}
}