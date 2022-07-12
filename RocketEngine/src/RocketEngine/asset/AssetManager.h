#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "RocketEngine/Defines.h"
#include <RKTUtils/Trackable.h>
#include <map>
#include <RocketEngine\component\TextComponent.h>

namespace RKTEngine
{
	class Texture2D;
	class Font;

	class AssetManager : public RKTUtil::Trackable
		{
		public:
			AssetManager();
			~AssetManager();

			bool initialize();

			void clean();

			Texture2D* loadSpriteAsset(const std::string& spriteName); //take model name and make it the key, add to map if not there, else return model ref
			Font* loadFontAsset(const std::string& fontName);

			std::pair<int, int> getSpriteAtlasIndex(std::string tileName);
			
			static const std::string sDEFAULT_FONT;
			static const std::string sDEFAULT_SPRITE;

			static const std::string sSPRITE_SHADER_ID;
			static const std::string sSPRITE_SHADER_FILENAME;
			static const std::string sTEXT_SHADER_ID;
			static const std::string sTEXT_SHADER_FILENAME;
			static const std::string sFRAG_SHADER_FILE_ENDING;
			static const std::string sVERT_SHADER_FILE_ENDING;

		private:
			const std::string mSPRITE_ASSET_PATH = "assets/sprites/";
			const std::string mSPRITE_FILE_ENDING = ".png";
			const std::string mFONT_ASSET_PATH = "assets/fonts/";
			const std::string mFONT_FILE_ENDING = ".ttf";

			std::map<std::string, std::pair<int,int>> mSpriteTileAssetCache;
			std::map<std::string, Texture2D*> mTextureAssetCache;
			std::map<std::string, Font*> mFontAssetCache;
	};
}
#endif // !ASSET_MANAGER_H
