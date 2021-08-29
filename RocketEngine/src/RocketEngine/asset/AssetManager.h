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

			inline const std::string& getSpriteShaderID() { return mSPRITE_SHADER_ID; }
			inline const std::string& getTextShaderID() { return mTEXT_SHADER_ID; }

		private:
			const std::string mSPRITE_ASSET_PATH = "assets/sprites/";
			const std::string mSPRITE_FILE_ENDING = ".png";
			const std::string mFONT_ASSET_PATH = "assets/fonts/";
			const std::string mSPRITE_SHADER_ID = "sprite";
			const std::string mTEXT_SHADER_ID = "text";

			std::map<std::string, std::pair<int,int>> mSpriteTileAssetCache;
			std::map<std::string, Texture2D*> mTextureAssetCache;
			std::map<std::string, Font*> mFontAssetCache;

			unsigned char* loadByteData(const std::string& path);
	};
}
#endif // !ASSET_MANAGER_H
