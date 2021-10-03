#ifndef DUNGEON_GEN_H
#define DUNGEON_GEN_H

#include <RKTUtils/Trackable.h>
#include <vector>
#include <glm/vec2.hpp>

struct Room
{
	int mTileHeight, mTileWidth;
	glm::vec2 position;
};

struct Node
{
	Room* mRoom = nullptr;
	
	Node* mLeftNode = nullptr;
	Node* mRightNode = nullptr;

	int mWidth, mHeight;
	glm::vec2 mTopLeft;

	Node(int width, int height, glm::vec2 topLeft) : 
		mWidth(width), mHeight(height), mTopLeft(topLeft), mLeftNode(nullptr), mRightNode(nullptr), mRoom(nullptr) {};
};

class DungeonGenerator : public RKTUtil::Trackable
{
	public:
		DungeonGenerator(int width, int height);
		~DungeonGenerator();

		void init(int width, int height);
		void cleanup();
		void cleanupBSPTree(Node* node);

		void generate(Node* node);
		void createRoom(Node* node);
		void createCorridor(Node* node);

		void storeRoomData(Node* node, int width, int height, glm::vec2 position);
		void storeCorridorData(int width, int height, glm::vec2 position);

		Room* getRoom(Node* node);

		int snapToTileGrid(int n, int x);

		const std::vector<Room*> getRoomDisplayData() { return mRoomDataList; }

	private:
		const int mVERTICAL_SLICE = 0;
		int mWidth, mHeight;
		int mMinWidth, mMinHeight;

		std::vector<Room*> mRoomDataList;
		Node* mRootNode = nullptr;
};

#endif // !DUNGEON_GEN_H
