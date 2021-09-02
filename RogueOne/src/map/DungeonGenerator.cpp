#include "DungeonGenerator.h"
#include <RocketEngine/core/Log.h>
#include <sstream>

DungeonGenerator::DungeonGenerator(int width, int height) :
	mWidth(width), mHeight(height), mMinWidth((int)(width * 0.33f)), mMinHeight((int)(height * 0.33f))
{
	std::random_device rd; // obtain a random number from hardware
	gen = std::mt19937(rd()); // seed the generator
	gen.seed((unsigned)time(NULL));
	init(width, height);
}

DungeonGenerator::~DungeonGenerator()
{
	cleanup();
}

void DungeonGenerator::init(int width, int height)
{
	mRootNode = new Node(width, height, glm::vec2(0, 0));
	slice(mRootNode);

//	displayTree(mRootNode, 0);
}

void DungeonGenerator::cleanup()
{
	for (auto room : mRoomDataList)
	{
		delete room;
	}

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

void DungeonGenerator::slice(Node* node)
{
	std::uniform_int_distribution<> randomAxis(0, 1); // define the range
	std::uniform_int_distribution<> randomDungeonCoordX(node->mWidth / 6, (int)(node->mWidth * .75f)); // define the range
	std::uniform_int_distribution<> randomDungeonCoordY(node->mHeight / 6, (int)(node->mHeight*.75f)); // define the range

	int axis = randomAxis(gen);

	std::stringstream ss;
	auto str = axis == 0 ? "Vertical" : "Horizontal";
	ss << "split: " << str; //<< ", width: " << node->mWidth << ", height: " << node->mHeight;
	RKT_TRACE(ss.str());

	if (axis == mVERTICAL_SLICE)
	{
		int splitXPos = randomDungeonCoordX(gen);
 		
		//divide the area along a horizontal or vertical line
		int leftNodeWidth = node->mWidth - splitXPos;
		int rightNodeWidth = splitXPos;
		int nodeHeight = node->mHeight;

		//validate node size
		//if (leftNodeWidth > mMinWidth)
		//{
			//int x = node->mTopLeft.x + (node->mWidth - width);
			//int y = node->mTopLeft.y;

			//create node
			Node* newLeftNode = new Node(leftNodeWidth, nodeHeight, node->mTopLeft);
			node->mLeftNode = newLeftNode;

			//slice
			//slice(newLeftNode);
		//}
		//else
		//{
			//createRoom(node);
		//}


		//if (rightNodeWidth > mMinWidth)
		//{
			//create node
			glm::vec2 topLeft = glm::vec2(node->mTopLeft.x + (node->mWidth - splitXPos), node->mTopLeft.y);
			Node* newRightNode = new Node(rightNodeWidth, nodeHeight, topLeft);
			node->mRightNode = newRightNode;

			//slice
			//slice(newRightNode);
		//}
		//else
		//{
			//createRoom(node);
		//}
	}
	else
	{
		int splitYPos = randomDungeonCoordY(gen);

		int topNodeHeight = node->mHeight - splitYPos;
		int bottomNodeHeight = splitYPos;
		int nodeWidth = node->mWidth;

		//validate node size
		//if (topNodeHeight > mMinHeight)
		//{
			Node* newLeftNode = new Node(nodeWidth, topNodeHeight, node->mTopLeft);
			node->mLeftNode = newLeftNode;

			//slice
			//slice(newLeftNode);
		//}
		//else
		//{
			//createRoom(node);
		//}

		//if (bottomNodeHeight > mMinHeight)
		//{
			glm::vec2 topLeft = glm::vec2(node->mTopLeft.x, node->mTopLeft.y + (node->mHeight - splitYPos));
			Node* newRightNode = new Node(nodeWidth, bottomNodeHeight, topLeft);
			node->mRightNode = newRightNode;

			//slice
			//slice(newRightNode);
		//}
		//else
		//{
			//createRoom(node);
		//}
	}

	if (axis == mVERTICAL_SLICE)
	{
		if (node->mLeftNode->mWidth > mMinWidth)
		{	
			slice(node->mLeftNode);
		}
		else
		{
			createRoom(node->mLeftNode);
		}

		if (node->mRightNode->mWidth > mMinWidth)
		{
			slice(node->mRightNode);
		}
		else
		{
			createRoom(node->mRightNode);
		}
	}
	else
	{
		if (node->mLeftNode->mHeight > mMinHeight)
		{
			slice(node->mLeftNode);
		}
		else
		{
			createRoom(node->mLeftNode);
		}

		if (node->mRightNode->mHeight > mMinHeight)
		{
			slice(node->mRightNode);
		}
		else
		{
			createRoom(node->mRightNode);
		}
	}
}

void DungeonGenerator::createRoom(Node* node)
{
	/*
*	TODO: create rooms
*	create a room within the cell by randomly
*	choosing two points (top left and bottom right)
*	within its boundaries
*/

	int roomWidth, roomHeight;

	std::uniform_int_distribution<int> w;
	std::uniform_int_distribution<int> h;

	//Encourage usage of wider/taller node space
	if (node->mWidth > node->mHeight)
	{
		w = std::uniform_int_distribution<int>((node->mWidth) / 2, node->mWidth);
		h = std::uniform_int_distribution<int>((node->mHeight) / 4, node->mHeight);
	}
	else
	{
		w = std::uniform_int_distribution<int>((node->mWidth) / 4, node->mWidth);
		h = std::uniform_int_distribution<int>((node->mHeight) / 2, node->mHeight);
	}

	roomWidth = w(gen);
	roomHeight = h(gen);

	RKT_TRACE("pixel width: " + std::to_string(roomWidth) + "\tpixel height: " + std::to_string(roomHeight));
	//RKT_TRACE("Num tiles: " + std::to_string(roomWidth / 16) );

	//generators for choosing two points(top left and bottom right)
	std::uniform_int_distribution<> randomRoomX((int)node->mTopLeft.x, (int)(node->mTopLeft.x + (node->mWidth - roomWidth)));
	std::uniform_int_distribution<> randomRoomY((int)node->mTopLeft.y, (int)(node->mTopLeft.y + (node->mHeight - roomHeight)));

	glm::vec2 topLeft = glm::vec2(randomRoomX(gen), randomRoomY(gen));

	//Store room data
	Room* newRoom = new Room;
	//Flipped height and width storage due to map tiling order
	newRoom->mRoomHeight = std::clamp(roomHeight / 16, 3, 15);
	newRoom->mRoomWidth = std::clamp(roomWidth / 16, 3, 15);
	newRoom->position = topLeft;

	RKT_TRACE("tile width: " + std::to_string(newRoom->mRoomWidth) + "\tpixel height: " + std::to_string(newRoom->mRoomHeight));

	mRoomDataList.push_back(newRoom);
}

