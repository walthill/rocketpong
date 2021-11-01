#ifndef MAP_H
#define MAP_H

#include "actors/Actor.h"
#include <vector>
#include <glm/vec2.hpp>

class Tile : public Actor
{
	public:
		Tile(glm::vec2 roomTopLeft, int tileX, int tileY, bool isHeightNegative, bool canWalk = true);
		bool mCanWalk;
		int mTileX, mTileY; //Tile location in the map array
};

struct Room;

class Map : public RKTUtil::Trackable
{
	public:
		Map(int width, int height, int tileSize);
		~Map();

		void clearMap();
		void addRoom(const Room* room);
		void makeWall(int x, int y);

		bool isWall(int x, int y) const;
		int getHeight() const { return mHeight; }
		int getWidth() const { return mWidth; }

		bool isValidPosition(glm::vec2 pos);

		int startX = 0, startY = 0;
		static int sTileSize;
	private:
		std::vector<Tile*> mMapTiles;
		std::vector<Tile*> mOverflowVisualTiles; //render overlapping tiles w/o affecting traversa
		int mWidth, mHeight;
};


#endif // !MAP_H
