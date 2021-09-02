#ifndef DUNGEON_GEN_H
#define DUNGEON_GEN_H

#include <RKTUtils/Trackable.h>
#include <random>
#include <vector>
#include <glm/vec2.hpp>

struct Room
{
	int mRoomHeight, mRoomWidth;
	glm::vec2 position;
};

struct Node
{
	//Node data
	Node* mLeftNode = nullptr;
	Node* mRightNode = nullptr;
	int mWidth, mHeight;
	glm::vec2 mTopLeft;

	Node(int width, int height, glm::vec2 topLeft) : mWidth(width), mHeight(height), mTopLeft(topLeft), mLeftNode(nullptr), mRightNode(nullptr) {};
};

class DungeonGenerator : public RKTUtil::Trackable
{
	public:
		DungeonGenerator(int width, int height);
		~DungeonGenerator();

		void init(int width, int height);
		void cleanup();
		void cleanupBSPTree(Node* node);

		void slice(Node* node);
		void createRoom(Node* node);

		const std::vector<Room*> getRoomDisplayData() { return mRoomDataList; }

	private:
		std::mt19937 gen;
		const int mVERTICAL_SLICE = 0;
		int mWidth, mHeight;
		int mMinWidth, mMinHeight;

		std::vector<Room*> mRoomDataList;
		Node* mRootNode = nullptr;
};

#endif // !DUNGEON_GEN_H
