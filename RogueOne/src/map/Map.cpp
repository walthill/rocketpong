#include "Map.h"
#include "RocketEngine.h"
#include "map/DungeonGenerator.h"

int Map::sTileSize = 16;

Map::Map(int width, int height, int tileSize) :
	mWidth(width), mHeight(height)
{
	sTileSize = tileSize;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			mMapTiles.push_back(new Tile(i, j, *this));
		}
	}

	//makeWall(5, 5);
	//makeWall(8, 8);
}

Map::Map(const Room* room) :
	mWidth(room->mTileWidth), mHeight(room->mTileHeight)
{
	startX = (int)room->position.x;
	startY = (int)room->position.y;

	int absHeight = mHeight;
	if (absHeight < 0)
	{
		absHeight = std::abs(absHeight);
	}

	for (int i = 0; i < mWidth; i++)
	{
		for (int j = 0; j < absHeight; j++)
		{
			mMapTiles.push_back(new Tile(i, j, *this));
		}
	}
}

Map::~Map()
{
	int absHeight = mHeight;
	if (absHeight < 0)
		absHeight = std::abs(absHeight);

	for (int i = 0; i < mWidth * absHeight; i++)
	{
		delete mMapTiles[i];
	}
}

void Map::makeWall(int x, int y)
{
	size_t index = y + (x * mHeight);
	if (index < mMapTiles.size())
	{
		auto tile = mMapTiles[index];
		tile->mpGameObject->getSprite()->setSprite("wall");
		tile->mCanWalk = false;
	}
	else
	{
		RKT_WARN("Unable to place wall due to an out of bounds index");
	}
}

bool Map::isWall(int x, int y) const
{
	return !mMapTiles[y + (x * mHeight)]->mCanWalk;
}

bool Map::isValidPosition(glm::vec2 pos)
{
	if (pos.x < 0 || pos.y < 0)
		return false;

	//turn position into grid 
	int x = (int)(pos.x / sTileSize);
	int y = (int)(pos.y / sTileSize);

	if (x > mWidth - 1 || y > mHeight - 1)
		return false;

	return !isWall(x,y);
}

//~~~ TILE ~~~~//

Tile::Tile(int tileX, int tileY, const Map& map, bool canWalk) : 
	mCanWalk(true), mTileX(tileX), mTileY(tileY), mMapHandle(map)
{
	std::string spriteName;

	mCanWalk ? spriteName = "floor" : spriteName = "wall";

	int yOffset = mMapHandle.getHeight() < 0 ? -(tileY * Map::sTileSize) : (tileY * Map::sTileSize);

	mpGameObject = RocketEngine->getEntityManager()->createSprite("tileset_0", spriteName, 16, 16, glm::vec2(mMapHandle.startX + (tileX * Map::sTileSize), mMapHandle.startY + yOffset));
}
