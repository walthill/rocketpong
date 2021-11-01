#include "Map.h"
#include "RocketEngine.h"
#include "map/DungeonGenerator.h"

int Map::sTileSize = 16;

Map::Map(int width, int height, int tileSize) :
	mWidth(width / tileSize), mHeight(height / tileSize)
{
	sTileSize = tileSize;
	mOverflowVisualTiles = std::vector<Tile*>();

	for (int i = 0; i < mWidth * mHeight; i++)
	{
		mMapTiles.push_back(nullptr);
	}

	//makeWall(5, 5);
	//makeWall(8, 8);
}


Map::~Map()
{
	clearMap();
	mMapTiles.clear();
}

void Map::clearMap()
{
	for (int i = 0; i < mWidth * mHeight; i++)
	{
		if (mMapTiles[i] != nullptr)
		{
			delete mMapTiles[i];
			mMapTiles[i] = nullptr;
		}
	}
	for (auto& tile : mOverflowVisualTiles)
	{
		delete tile;
	}
	mOverflowVisualTiles.clear();
}


void Map::addRoom(const Room* room)
{
	startX = (int)room->position.x;
	startY = (int)room->position.y;

	//turn topLeft position into grid 
	int gridX = startX / sTileSize;
	int gridY = startY / sTileSize;

	int absHeight = room->mTileHeight;
	bool isHeightNegative = absHeight < 0;
	if (isHeightNegative)
	{
		absHeight = std::abs(absHeight);
	}

	for (int i = 0; i < room->mTileWidth; i++)
	{
		for (int j = 0; j < absHeight; j++)
		{
			int x = gridX + i;
			int y = gridY + j;
			int index = y + (x * mHeight);

			if (index < mWidth * mHeight)
			{
				if (mMapTiles[index] == nullptr)
				{
					mMapTiles[index] = new Tile(room->position, i, j, isHeightNegative);
				}
				else
				{
					mOverflowVisualTiles.push_back(new Tile(room->position, i, j, isHeightNegative));
				}
			}
		}
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
	bool isWall = true;

	auto tile = mMapTiles[y + (x * mHeight)];
	if (tile != nullptr)
		isWall = !tile->mCanWalk;

	return isWall;
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

Tile::Tile(glm::vec2 roomTopLeft, int tileX, int tileY, bool isHeightNegative, bool canWalk) :
	mCanWalk(true), mTileX(tileX), mTileY(tileY)
{
	std::string spriteName;

	mCanWalk ? spriteName = "floor" : spriteName = "wall";

	int xOffset = tileX * Map::sTileSize;
	int yOffset = isHeightNegative ? -(tileY * Map::sTileSize) : (tileY * Map::sTileSize);

	mpGameObject = RocketEngine->getEntityManager()->createSprite("tileset_0", spriteName, Map::sTileSize, Map::sTileSize, glm::vec2(roomTopLeft.x + xOffset, roomTopLeft.y + yOffset));
}
