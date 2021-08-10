#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "RocketEngine/Defines.h"
#include <RKTUtils/Trackable.h>
#include <map>

namespace RKTEngine
{
	class Texture2D;

	class AssetManager : public RKTUtil::Trackable
		{
		public:
			AssetManager();
			~AssetManager();

			bool initialize();

			void clean();

			Texture2D* loadSpriteAsset(std::string spriteName); //take model name and make it the key, add to map if not there, else return model ref
			unsigned char* loadFont(std::string fontName);

			inline const std::string& getSpriteShaderID() { return mSPRITE_SHADER_ID; }

		private:
			const std::string mSPRITE_ASSET_PATH = "assets/sprites/";
			const std::string mSPRITE_FILE_ENDING = ".png";
			const std::string mFONT_ASSET_PATH = "assets/fonts/";
			const std::string mSPRITE_SHADER_ID = "sprite";
			const std::string mTEXT_SHADER_ID = "text";

			std::map<std::string, Texture2D*> mSpriteAssetCache;
	};
}
#endif // !ASSET_MANAGER_H
