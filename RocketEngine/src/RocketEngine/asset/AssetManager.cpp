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

	Texture2D* AssetManager::loadSpriteAsset(std::string path)
	{
		const auto& key = path.substr(path.find_last_of('/') + 1, path.find_last_of('.'));
		const auto& iter = mSpriteAssetCache.find(key);

		if (iter != mSpriteAssetCache.end())
		{
			return iter->second;
		}
		else
		{
			Texture2D* newSprite = Texture2D::create(path);
			mSpriteAssetCache[key] = newSprite;

			return newSprite;
		}
	}
}