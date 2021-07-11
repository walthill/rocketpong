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

			Texture2D* loadSpriteAsset(std::string path); //take model name and make it the key, add to map if not there, else return model ref

		private:
			std::map<std::string, Texture2D*> mSpriteAssetCache;
	};
}
#endif // !ASSET_MANAGER_H
