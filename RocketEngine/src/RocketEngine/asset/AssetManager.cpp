#include "AssetManager.h"
#include <RocketEngine/core/Log.h>
#include "RocketEngine/render/buffers/Texture.h"
#include "RocketEngine/render/Font.h"
#include "RocketEngine/audio/AudioSource.h"
#include "Serialization.h"

namespace RKTEngine
{
	const std::string AssetManager::sDEFAULT_FONT = "calibri";
	const std::string AssetManager::sDEFAULT_SPRITE = "defaultcrab";
	const std::string AssetManager::sDEFAULT_BTN_SPRITE = "defaultbtn";
	const std::string AssetManager::sDEFAULT_BTN_HIGHLIGHT_SPRITE = "defaultbtn_highlight";

	const std::string AssetManager::sSPRITE_SHADER_ID = "sprite";
	const std::string AssetManager::sSPRITE_SHADER_FILENAME = "BatchTexture";
	const std::string AssetManager::sTEXT_SHADER_ID = "text";
	const std::string AssetManager::sTEXT_SHADER_FILENAME = "TextRender";
	const std::string AssetManager::sFRAG_SHADER_FILE_ENDING = ".frag";
	const std::string AssetManager::sVERT_SHADER_FILE_ENDING = ".vert";


	AssetManager::AssetManager()
	{
	}

	AssetManager::~AssetManager()
	{
		clean();
	}

	bool AssetManager::initialize()
	{
		mTextureAssetCache = std::map<std::string, Texture2D*>();
		return true;
	}

	void AssetManager::clean()
	{
		auto iter = mTextureAssetCache.begin();
		for (iter; iter != mTextureAssetCache.end(); ++iter)
		{
			delete iter->second;
		}

		auto iter2 = mFontAssetCache.begin();
		for (iter2; iter2 != mFontAssetCache.end(); ++iter2)
		{
			delete iter2->second;
		}

		auto iter3 = mAudioAssetCache.begin();
		for (iter3; iter3 != mAudioAssetCache.end(); ++iter3)
		{
			delete iter3->second;
		}

		mTextureAssetCache.clear();
		mFontAssetCache.clear();
		mAudioAssetCache.clear();
	}

	Texture2D* AssetManager::loadSpriteAsset(const std::string& spriteName)
	{
		Texture2D* newSprite = nullptr;
		const auto& iter = mTextureAssetCache.find(spriteName);
		if (iter != mTextureAssetCache.end())
		{
			newSprite = iter->second;
		}
		else
		{
			newSprite = Texture2D::create(mSPRITE_ASSET_PATH + spriteName + mSPRITE_FILE_ENDING);
			mTextureAssetCache[spriteName] = newSprite;
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
			auto path = mFONT_ASSET_PATH + fontName + mFONT_FILE_ENDING;
			newFont = Font::create(Serialization::loadByteData(path));
			mFontAssetCache[fontName] = newFont;
		}

		return newFont;
	}

	AudioSource* AssetManager::loadAudioSource(const std::string& audioFileName)
	{
		AudioSource* newAudio = nullptr;
		const auto& iter = mAudioAssetCache.find(audioFileName);
		if (iter != mAudioAssetCache.end())
		{
			newAudio = iter->second;
		}
		else
		{
			auto path = mAUDIO_ASSET_PATH + audioFileName + mAUDIO_WAV_FILE_ENDING;
			newAudio = AudioSource::create(path);
			mAudioAssetCache[audioFileName] = newAudio;
		}

		return newAudio;
	}

	std::pair<int, int> AssetManager::getSpriteAtlasIndex(std::string tileName)
	{
		std::pair<int, int> offset;

		const auto& iter = mSpriteTileAssetCache.find(tileName);
		if (iter != mSpriteTileAssetCache.end())
		{
			offset = iter->second;
		}
		else
		{
			RKT_CORE_ERROR("ERROR::Cannot find tile name: " + tileName);
		}

		return offset;
	}

}