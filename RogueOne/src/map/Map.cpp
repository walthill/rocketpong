#include "Map.h"
#include "RocketEngine.h"

int Map::sTileSize = 16;
int Map::mStartX = 0;
int Map::mStartY = 0;

Map::Map(int width, int height, int tileSize) :
	mWidth(width), mHeight(height)
{
	sTileSize = tileSize;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			mMapTiles.push_back(new Tile(i, j));
		}
	}

	makeWall(1, 0);
	makeWall(5, 5);
	makeWall(8, 8);
}

Map::~Map()
{
	for (int i = 0; i < mWidth * mHeight; i++)
	{
		delete mMapTiles[i];
	}
}

void Map::makeWall(int x, int y)
{
	auto tile = mMapTiles[x + y * mWidth];
	tile->mpGameObject->getSprite()->setSprite("wall");
	tile->mCanWalk = false;
}

bool Map::isValidPosition(glm::vec2 pos)
{
	//turn position into grid 
	int x = (int)(pos.x / sTileSize);
	int y = (int)(pos.y / sTileSize);

	return !isWall(x,y);
}

//~~~ TILE ~~~~//

Tile::Tile(int tileX, int tileY, bool canWalk) : 
	mCanWalk(true), mTileX(tileX), mTileY(tileY)
{
	std::string spriteName;

	mCanWalk ? spriteName = "floor" : spriteName = "wall";
	mpGameObject = RocketEngine->getEntityManager()->createSprite("tileset_0", spriteName, 16, 16, glm::vec2(Map::mStartX + (tileX * Map::sTileSize), Map::mStartY+ (tileY * Map::sTileSize)));
}
