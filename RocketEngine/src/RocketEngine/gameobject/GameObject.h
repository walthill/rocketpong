/********
	=========================
			 ROCKET3D
	=========================
	File Created By: Walter Hill

	Rocket3D is an open source 3D game engine written using C++ & OpenGL.

	This code is open source under the Apache 2.0 license.
	(https://github.com/walthill/Rocket3D/blob/master/LICENSE)

	=========================
		 GameObject.h
	=========================
	This file contains the definition for the GameObject class.
	The class handles calculations for GameObjects and holds id's 
	for components assigned to the GameObject.

********/

#ifndef GAME_OBJ_H
#define GAME_OBJ_H

#include "RocketEngine/Defines.h"
#include <RKTUtils/Trackable.h>

namespace RKTEngine
{
	class SpriteComponent;
	class TransformComponent;
	class TextComponent;

	/***************************************************************************//**
	 * @brief 	GameObject class with identifiers for components.
	 *
	 * This class contains a GameObject's component identifiers and controls the
	 * processing of transform data.
	 ******************************************************************************/
	class GameObject : public RKTUtil::Trackable
	{
		public:
			///Default constructor
			GameObject();
			///Default deconstructor
			~GameObject();

			///Cleanup
			void cleanup();

			/**********************************************************************//**
			* Process transform calulations and send data to Mesh Component.
			*
			* @param elapsedTime Frame timing
			*************************************************************************/
			void update(float elapsedTime);

			/**********************************************************************//**
			* Set GameObject's unique id
			*
			* @param id GameObject identifier
			*************************************************************************/
			void setId(uint32 id) { mId = id; }

			/**********************************************************************//**
			* Set GameObject's transform reference
			*
			* @param comp TransformComponent reference
			*************************************************************************/
			void setTransformHandle(TransformComponent* comp) { mpTransform = comp; }

			/**********************************************************************//**
			* Set GameObject's transform id
			*
			* @param transfromId Component identifier
			*************************************************************************/
			void connectTransform(ComponentId transfromId) { mTransformId = transfromId; }

			/**********************************************************************//**
			* Set GameObject's sprite id
			*
			* @param spriteId SpriteComponent identifier
			*************************************************************************/
			void connectSprite(ComponentId spriteId) { mSpriteId = spriteId; }

			/**********************************************************************//**
			* Set GameObject's label id
			*
			* @param labelId TextComponent identifier
			*************************************************************************/
			void connectLabel(ComponentId labelId) { mLabelId = labelId; }


			///Access the GameObject's id
			inline uint32 getId() { return mId; }
			///Access the GameObject's transform component id
			inline ComponentId getTransformId() { return mTransformId; };
			///Access the GameObject's mesh component id
			inline ComponentId getSpriteId() { return mSpriteId; };
			///Access the GameObject's text component id
			inline ComponentId getLabelId() { return mLabelId; };

			///Acesss the GameObject's transform 
			TransformComponent* getTransform() { return mpTransform; }
			///Acesss the GameObject's mesh 
			SpriteComponent* getSprite();
			///Acesss the GameObject's material
			TextComponent* getUILabel();

			std::string name = "New Gameobject";

		private:
			uint32 mId;
			ComponentId mSpriteId;
			ComponentId mTransformId;
			ComponentId mLabelId;

			TransformComponent* mpTransform;
	};
}
#endif // !GAME_OBJ_H
