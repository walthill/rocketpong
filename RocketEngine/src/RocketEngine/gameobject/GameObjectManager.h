/********
	=========================
			 ROCKET3D
	=========================
	File Created By: Walter Hill

	Rocket3D is an open source 3D game engine written using C++ & OpenGL.

	This code is open source under the Apache 2.0 license.
	(https://github.com/walthill/Rocket3D/blob/master/LICENSE)

	=========================
		 GameObjectManager.h
	=========================
	This file contains the definition for the GameObject Manager class.
	The class handles calculations and rendering for all GameObjects
********/

#ifndef GAME_OBJ_MAN_H
#define GAME_OBJ_MAN_H

#include "RocketEngine/Defines.h"
#include <map>
#include <RKTUtils/DeanLib_MemoryPool.h>
#include "RocketEngine/component/TransformComponent.h"
#include "RocketEngine/component/SpriteComponent.h"

namespace RKTEngine
{
	class GameObject;

	const GameObjectId PLAYER_OBJ_ID = 0;
	const GameObjectId INVALID_GAMEOBJECT_ID = UINT_MAX;

	/***************************************************************************//**
	 * @brief 	The manager for all GameObjects.
	 *
	 * This class, contains all GameObjects and controls their
	 * rendering, processing, creation, and deletion.
	 ******************************************************************************/
	class GameObjectManager : public RKTUtil::Trackable
	{
		public:

			/**********************************************************************//**
			* Constructor for the manager.
			*
			* @param maxSize Sets the max number of GameObjects
			*************************************************************************/
			GameObjectManager(uint32 maxSize);

			///Default deconstructor
			~GameObjectManager();

			/**********************************************************************//**
			* Find and return a reference to a GameObject based on its id
			*
			* @param id The unique identifier for the GameObject to find
			*************************************************************************/
			GameObject* findGameObject(const GameObjectId& id) const;


			/**********************************************************************//**
			* Create a gameObject and return a referecne to that object.
			*
			* @param transform Data for the GameObject's transform. Defaults to a transform with posiion (0,0,0).
			* @param meshData Data for the GameObject's model. Defaults to a empty MeshComponent.
			* @param id The identifier for the GameObject. Defaults to an invalid value and is assigned in the function
			*************************************************************************/
			GameObject* createGameObject(const TransformData& transform = ZERO_TRANSFORM_DATA,
										 const SpriteComponentData& spriteData = ZERO_MESH_DATA,
						 				 const GameObjectId& id = INVALID_GAMEOBJECT_ID);

			/**********************************************************************//**
			* Destroy GameObject and remove from manager based on its id
			*
			* @param id The unique identifier for the GameObject to destroy
			*************************************************************************/
			void cleanup(const GameObjectId& id);

			void updateAll(float elapsedTime);

			///Get the number of gameobjects
			int getNumGameObjects();
			///Access the player gameobject
			GameObject* getPlayerObject() { return mGameObjMap[PLAYER_OBJ_ID]; }

		private:
			static GameObjectId msNextUnitId;

			RKTUtil::MemoryPool mGameObjectPool;
			std::map<GameObjectId, GameObject*> mGameObjMap;
	};
}
#endif // !GAME_OBJ_MAN_H