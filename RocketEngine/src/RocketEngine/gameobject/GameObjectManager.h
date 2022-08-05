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
#include "RocketEngine/component/TextComponent.h"
#include "RocketEngine/component/BoxColliderComponent.h"
#include "RocketEngine/input/message/Message.h"
#include "RocketEngine/asset/AssetManager.h"

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
		friend class EngineCore;
		public:

			/**********************************************************************//**
			* Constructor for the manager.
			*
			* @param maxSize Sets the max number of GameObjects
			*************************************************************************/
			GameObjectManager(uint32 maxSize);

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
										 const SpriteComponentData& spriteData = ZERO_SPRITE_DATA,
										 const TextData& labelData = ZERO_LABEL_DATA,
						 				 const GameObjectId& id = INVALID_GAMEOBJECT_ID);
			
			GameObject* createActor(const TransformData& transform = ZERO_TRANSFORM_DATA,
				const SpriteComponentData& spriteData = ZERO_SPRITE_DATA,
				const TextData& labelData = ZERO_LABEL_DATA,
				const GameObjectId& id = INVALID_GAMEOBJECT_ID);

#ifdef ATLASED_CREATE_FUNCS	
			GameObject* createAtlasedSprite(const std::string& spriteToLoad, const std::string& tileName, int width, int height, glm::vec2 position = glm::vec2(0, 0),
				glm::vec2 scale = glm::vec2(1, 1), float rotation = 0);

			GameObject* createAtlasedPlayer(const std::string& texture, const std::string& tileName, int width, int height, glm::vec2 position = glm::vec2(0, 0),
				glm::vec2 scale = glm::vec2(1, 1), float rotation = 0);
#endif

			GameObject* createSprite(const std::string& spriteToLoad = AssetManager::sDEFAULT_SPRITE, glm::vec2 position = glm::vec2(0,0),
									 glm::vec2 scale = glm::vec2(1, 1), float rotation = 0);

			GameObject* createPlayer(const std::string& texture, glm::vec2 position = glm::vec2(0, 0),
									 glm::vec2 scale = glm::vec2(1, 1), float rotation = 0);
			
			GameObject* createLabel(const std::string& text, glm::vec2 position = glm::vec2(0, 0), glm::vec2 scale = glm::vec2(1, 1), 
									float rotation = 0, const std::string& fontName = AssetManager::sDEFAULT_FONT, int fontSize = Text::sDefaultTextSize);

			void addAudioSource(int objId, const std::string& audio, float vol = -1.0f, float pan = 0.0f);
			void addBoxCollider(int objId, int w, int h, const std::string& t = "untagged");
			void addSprite(int objId, const std::string& spriteToLoad = AssetManager::sDEFAULT_SPRITE, const std::string& tileName = "", glm::vec2 position = glm::vec2(0, 0),
						   glm::vec2 scale = glm::vec2(1, 1), float rotation = 0);
			void addNativeScript(int objId);

			/**********************************************************************//**
			* Destroy GameObject and remove from manager based on its id
			*
			* @param id The unique identifier for the GameObject to destroy
			*************************************************************************/
			void destroy(const GameObjectId& id);

			///Get the number of gameobjects
			int getNumGameObjects();
			///Access the player gameobject
			GameObject* getPlayerObject() { return mGameObjMap[PLAYER_OBJ_ID]; }

		private:
			static GameObjectId msNextUnitId;

			RKTUtil::MemoryPool mGameObjectPool;
			std::map<GameObjectId, GameObject*> mGameObjMap;

			///Default deconstructor
			~GameObjectManager();

			void updateAll(float elapsedTime);
	};
}
#endif // !GAME_OBJ_MAN_H
