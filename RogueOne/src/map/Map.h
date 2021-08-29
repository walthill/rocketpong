#ifndef MAP_H
#define MAP_H

#include "../actors/Actor.h"
#include <vector>

class Tile : public Actor
{
	public:
		Tile(int tileX, int tileY, bool canWalk = true);

		bool mCanWalk;
		int mTileX, mTileY; //Tile location in the map array
};

class Map
{
	public:
		Map(int width, int height, int tileSize);
		~Map();

		void makeWall(int x, int y);

		inline bool Map::isWall(int x, int y) const { return !mMapTiles[x + y * mWidth]->mCanWalk; }
		inline int getHeight() { return mHeight; }
		inline int getWidth() { return mWidth; }

		static int sTileSize;
		static int mStartX, mStartY;
private:
		std::vector<Tile*> mMapTiles;
		int mWidth, mHeight;
};


#endif // !MAP_H
