#include "DungeonGenerator.h"
#include <RocketEngine/core/Log.h>
#include <RocketEngine/math/Random.h>

using Random = RKTEngine::Random;

DungeonGenerator::DungeonGenerator(int width, int height) :
	mWidth(width), mHeight(height), mMinWidth((int)(width * 0.25f)), mMinHeight((int)(height * 0.25f))
{
}

DungeonGenerator::~DungeonGenerator()
{
	cleanup();
}

void DungeonGenerator::generate()
{
	mRootNode = new Node(mWidth, mHeight, glm::vec2(0, 0));
	generate(mRootNode);
}

void DungeonGenerator::cleanup()
{
	for (auto room : mRoomDataList)
	{
		delete room;
	}
	mRoomDataList.clear();

	cleanupBSPTree(mRootNode);
}

void DungeonGenerator::cleanupBSPTree(Node* node)
{ 
	if (node == nullptr) 
		return;

	cleanupBSPTree(node->mLeftNode);
	cleanupBSPTree(node->mRightNode);

	delete node;
	node = nullptr;
}

void DungeonGenerator::generate(Node* node)
{
	int axis = Random::range(0, 1);
	if (axis == mVERTICAL_SLICE)
	{
		int splitXPos = Random::range(node->mWidth / 8, (int)(node->mWidth * .8f));
 		
		//divide the area along a horizontal or vertical line
		int leftNodeWidth = node->mWidth - splitXPos;
		int rightNodeWidth = splitXPos;
		int nodeHeight = node->mHeight;

		//create node
		Node* newLeftNode = new Node(leftNodeWidth, nodeHeight, node->mTopLeft);
		node->mLeftNode = newLeftNode;

		glm::vec2 topLeft = glm::vec2(node->mTopLeft.x + (node->mWidth - splitXPos), node->mTopLeft.y);
		Node* newRightNode = new Node(rightNodeWidth, nodeHeight, topLeft);
		node->mRightNode = newRightNode;

		if (node->mLeftNode->mWidth > mMinWidth)
		{
			generate(node->mLeftNode);
		}
		else
		{
			createRoom(node->mLeftNode);
		}

		if (node->mRightNode->mWidth > mMinWidth)
		{
			generate(node->mRightNode);	
		}
		else
		{
			createRoom(node->mRightNode);
		}

		if (node->mRightNode != nullptr && node->mLeftNode != nullptr)
		{
			createCorridor(node);
		}
	}
	else
	{
		int splitYPos = Random::range(node->mHeight / 8, (int)(node->mHeight * .8f));

		int topNodeHeight = node->mHeight - splitYPos;
		int bottomNodeHeight = splitYPos;
		int nodeWidth = node->mWidth;

		Node* newLeftNode = new Node(nodeWidth, topNodeHeight, node->mTopLeft);
		node->mLeftNode = newLeftNode;

		glm::vec2 topLeft = glm::vec2(node->mTopLeft.x, node->mTopLeft.y + (node->mHeight - splitYPos));
		Node* newRightNode = new Node(nodeWidth, bottomNodeHeight, topLeft);
		node->mRightNode = newRightNode;


		if (node->mLeftNode->mHeight > mMinHeight)
		{
			generate(node->mLeftNode);
		}
		else
		{
			createRoom(node->mLeftNode);
		}

		if (node->mRightNode->mHeight > mMinHeight)
		{
			generate(node->mRightNode);
		}
		else
		{
			createRoom(node->mRightNode);
		}

		if (node->mRightNode != nullptr && node->mLeftNode != nullptr)
		{
			createCorridor(node);
		}
	}
}

void DungeonGenerator::createRoom(Node* node)
{
	int roomWidth, roomHeight;
	glm::vec2 topLeft;

	/*//Encourage usage of wider/taller node space
	if (node->mWidth > node->mHeight)
	{
		roomWidth = Random::range((node->mWidth) / 4, node->mWidth);
		roomHeight = Random::range((node->mHeight) / 4, node->mHeight);
	}
	else
	{
		roomWidth = Random::range((node->mWidth) / 4, node->mWidth);
		roomHeight = Random::range((node->mHeight) / 4, node->mHeight);
	}*/

	roomWidth = Random::range((int)(node->mWidth * 0.25f), node->mWidth);
	roomHeight = Random::range((int)(node->mHeight * 0.25f), node->mHeight);

	int leftX = Random::range((int)node->mTopLeft.x, (int)(node->mTopLeft.x + (node->mWidth - roomWidth)));
	int leftY = Random::range((int)node->mTopLeft.y, (int)(node->mTopLeft.y + (node->mHeight - roomHeight)));
	
	leftX = snapToTileGrid(leftX, 16);
	leftY = snapToTileGrid(leftY, 16);
	
	topLeft = glm::vec2(leftX, leftY);
	
	storeRoomData(node, roomWidth / 16, roomHeight / 16, topLeft);
}


//God bless Mr. Romain Beaudon - http://www.rombdn.com/blog/2018/01/12/random-dungeon-bsp-unity/
void DungeonGenerator::createCorridor(Node* node)
{
	Room* leftRoom = getRoom(node->mLeftNode);
	Room* rightRoom = getRoom(node->mRightNode);

	// attach the corridor to a random point in each room
	int leftX = Random::range((int)leftRoom->position.x + 16, (int)leftRoom->position.x + (leftRoom->mTileWidth * 16) - 16);
	int leftY = Random::range((int)leftRoom->position.y + 16, (int)leftRoom->position.y + (leftRoom->mTileHeight * 16) - 16);

	leftX = snapToTileGrid(leftX, 16);
	leftY = snapToTileGrid(leftY, 16);

	int rightX = Random::range((int)rightRoom->position.x + 16, (int)rightRoom->position.x + (rightRoom->mTileWidth * 16) - 16);
	int rightY = Random::range((int)rightRoom->position.y + 16, (int)rightRoom->position.y + (rightRoom->mTileHeight * 16) - 16);

	rightX = snapToTileGrid(rightX, 16);
	rightY = snapToTileGrid(rightY, 16);

	glm::vec2 leftPoint = glm::vec2(leftX, leftY);
	glm::vec2 rightPoint = glm::vec2(rightX, rightY);

	// always be sure that left point is on the left to simplify the code
	if (leftPoint.x > rightPoint.x)
	{
		glm::vec2 temp = leftPoint;
		leftPoint = rightPoint;
		rightPoint = temp;
	}

	//find positional difference, convert to tile space
	int diffWidth = (int)(leftPoint.x - rightPoint.x)/16;
	int diffHeight = (int)(leftPoint.y - rightPoint.y)/16;

	// if the points are not aligned horizontally
	if (diffWidth != 0)
	{
		// choose at random to go horizontal then vertical or the opposite
		if (Random::range_real(0.0f,1.0f) > 0.5f) 
		{
			// add a corridor to the right
			storeCorridorData(std::abs(diffWidth), 1, leftPoint);
			// if left point is below right point go up
			// otherwise go down
			if (diffHeight < 0) 
			{
				storeCorridorData(1, std::abs(diffHeight), glm::vec2(rightPoint.x, leftPoint.y));
			}
			else 
			{
				storeCorridorData(1, -std::abs(diffHeight) , glm::vec2(rightPoint.x, leftPoint.y));
			}
		}
		else 
		{
			// go up or down
			if (diffHeight < 0)
			{
				storeCorridorData(1, std::abs(diffHeight), leftPoint);
			}
			else 
			{
				storeCorridorData(1, std::abs(diffHeight), glm::vec2(leftPoint.x, rightPoint.y));
			}

			// then go right
			storeCorridorData(std::abs(diffWidth), 1, glm::vec2(leftPoint.x, rightPoint.y));
		}
	}
	else 
	{
		// if the points are aligned horizontally
		// go up or down depending on the positions
		if (diffHeight < 0)
		{
			storeCorridorData(1, std::abs(diffHeight), leftPoint);
		}
		else 
		{
			storeCorridorData(1, std::abs(diffHeight), rightPoint);
		}
	}
}

void DungeonGenerator::storeRoomData(Node* node, int width, int height, glm::vec2 position)
{
	const int maxTiles = 15;
	const int minTiles = 3;

	Room* newRoom = new Room;
	newRoom->mTileHeight = std::clamp(height, minTiles, maxTiles);
	newRoom->mTileWidth = std::clamp(width, minTiles, maxTiles);
	newRoom->position = position;

	node->mRoom = newRoom;
	mRoomDataList.push_back(newRoom);
}

void DungeonGenerator::storeCorridorData(int width, int height, glm::vec2 position)
{
	Room* newCorridor = new Room; 
	newCorridor->mTileHeight = height;
	newCorridor->mTileWidth = width;
	newCorridor->position = position;

	mRoomDataList.push_back(newCorridor);
}

Room* DungeonGenerator::getRoom(Node* node)
{
	//Node has room
	if (node->mRoom != nullptr)
		return node->mRoom;

	//Node does NOT have room, find room in child
	//NOTE: at this stage, all node leaves should contain rooms
	if (node->mLeftNode != nullptr)
		return getRoom(node->mLeftNode);

	if (node->mRightNode != nullptr)
		return getRoom(node->mRightNode);

	RKT_ASSERT("Tree has no rooms! You may have missed a step in the generation process");

	return nullptr;
}


// Function to calculate the smallest multiple
int DungeonGenerator::snapToTileGrid(int value, int tileSize)
{
	if (tileSize > value)
		return tileSize;

	value = value + tileSize / 2;
	value = value - (value % tileSize);
	return value;
}