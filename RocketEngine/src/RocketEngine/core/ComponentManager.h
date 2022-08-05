/********
	=========================
			 ROCKET3D
	=========================
	File Created By: Walter Hill

	Rocket3D is an open source 3D game engine written using C++ & OpenGL.

	This code is open source under the Apache 2.0 license.
	(https://github.com/walthill/Rocket3D/blob/master/LICENSE)

	=========================
		 ComponentManager.h
	=========================
	This file contains the definition for the ComponentManager class.
	The class handles calculations and rendering for components 
	and holds all components allocated to GameObjects.

********/

#ifndef COMPONENT_MAN_H
#define COMPONENT_MAN_H

#include <map>
#include <RKTUtils/DeanLib_MemoryPool.h>
#include "RocketEngine/component/SpriteComponent.h"
#include "RocketEngine/component/TransformComponent.h"
#include "RocketEngine/component/TextComponent.h"
#include "RocketEngine/component/BoxColliderComponent.h"
#include "RocketEngine/component/AudioSourceComponent.h"
#include "RocketEngine/component/NativeScriptComponent.h"
#include <RocketEngine/input/message/Message.h>

namespace RKTEngine
{

	class ShaderManager;
	/***************************************************************************//**
	 * @brief 	Stores collection of components.
	 *
	 * Stores collection of components. and handles their processing and rendering
	 ******************************************************************************/
	class ComponentManager : public RKTUtil::Trackable
	{
	public:
		/**********************************************************************//**
		* Process components and perform calulations.
		*
		* @param maxSize Sets maximum number of components
		* @param shaderMan Refernce to the shader manager
		* @param lightingShaderKey Identifier for the shader used in lighting components
		*************************************************************************/
		ComponentManager(uint32 maxSize);

		///Default decconstructor
		~ComponentManager();

		///Cleanup
		void clean();


//------	TRANSFORM	------ //


		/**********************************************************************//**
		* Access the transform component based on the component identifier.
		*
		* @param id Component identifier
		*************************************************************************/
		TransformComponent* getTransformComponent(const ComponentId& id);

		/**********************************************************************//**
		* Create transform component based on data passed in and return the component identifier.
		*
		* @param data data Transform component data
		*************************************************************************/
		ComponentId allocateTransformComponent(const TransformData& data = ZERO_TRANSFORM_DATA);

		/**********************************************************************//**
		* Remove and destroy transform component from the collection based on the component identifier.
		*
		* @param id Component identifier
		*************************************************************************/
		void deallocateTransformComponent(const ComponentId& id);


//------	SPRITE	------ //


		/**********************************************************************//**
		* Access the mesh component based on its component identifier.
		*
		* @param id Component identifier
		*************************************************************************/
		SpriteComponent* getSpriteComponent(const ComponentId& id);

		/**********************************************************************//**
		* Create mesh component based on data passed in and return the component identifier.
		*
		* @param meshId mesh component identifier
		* @param data data Mesh component data
		*************************************************************************/
		ComponentId allocateSpriteComponent(const SpriteComponentData& data = ZERO_SPRITE_DATA);

		/**********************************************************************//**
		* Remove and destroy mesh component from the collection based on the component identifier.
		*
		* @param id Component identifier
		*************************************************************************/
		void deallocateSpriteComponent(const ComponentId& id);


//------	TEXT	------ //

		/**********************************************************************//**
		* Access the transform component based on the component identifier.
		*
		* @param id Component identifier
		*************************************************************************/
		TextComponent* getTextComponent(const ComponentId& id);

		/**********************************************************************//**
		* Create transform component based on data passed in and return the component identifier.
		*
		* @param data data Transform component data
		*************************************************************************/
		ComponentId allocateTextComponent(const ComponentId& labelID, const TextData& data = ZERO_LABEL_DATA);

		/**********************************************************************//**
		* Remove and destroy transform component from the collection based on the component identifier.
		*
		* @param id Component identifier
		*************************************************************************/
		void deallocateTextComponent(const ComponentId& id);


//------	BOX COLLIDER	------ //

		/**********************************************************************//**
		* Access the transform component based on the component identifier.
		*
		* @param id Component identifier
		*************************************************************************/
		BoxColliderComponent* getBoxColliderComponent(const ComponentId& id);

		/**********************************************************************//**
		* Create transform component based on data passed in and return the component identifier.
		*
		* @param data data Transform component data
		*************************************************************************/
		ComponentId allocateBoxColliderComponent(const BoxColliderData& data = ZERO_BOX_COLLIDER_DATA);

		/**********************************************************************//**
		* Remove and destroy transform component from the collection based on the component identifier.
		*
		* @param id Component identifier
		*************************************************************************/
		void deallocateBoxColliderComponent(const ComponentId& id);



//------	AUDIO SOURCE COMPONENT------ //

		/**********************************************************************//**
		* Access the transform component based on the component identifier.
		*
		* @param id Component identifier
		*************************************************************************/
		AudioSourceComponent* getAudioSourceComponent(const ComponentId& id);

		/**********************************************************************//**
		* Create transform component based on data passed in and return the component identifier.
		*
		* @param data data Transform component data
		*************************************************************************/
		ComponentId allocateAudioSourceComponent(const AudioSourceComponentData& data = ZERO_AUDIO_SRC_DATA);

		/**********************************************************************//**
		* Remove and destroy transform component from the collection based on the component identifier.
		*
		* @param id Component identifier
		*************************************************************************/
		void deallocateAudioSourceComponent(const ComponentId& id);

//------	NATIVE SCRIPT COMPONENT------ //

		/**********************************************************************//**
		* Access the transform component based on the component identifier.
		*
		* @param id Component identifier
		*************************************************************************/
		NativeScriptComponent* getNativeScriptComponent(const ComponentId& id);

		/**********************************************************************//**
		* Create transform component based on data passed in and return the component identifier.
		*
		* @param data data Transform component data
		*************************************************************************/
		ComponentId allocateNativeScriptComponent(const uint32& id);

		/**********************************************************************//**
		* Remove and destroy transform component from the collection based on the component identifier.
		*
		* @param id Component identifier
		*************************************************************************/
		void deallocateNativeScriptComponent(const ComponentId& id);


		/**********************************************************************//**
		* Process components and perform calulations.
		*
		* @param elapsedTime Frame timing
		*************************************************************************/
		void update(float elapsedTime);
		void updateCollisions();
		void updateScripts();

		void onMessage(Message& message);

		void renderComponents();

		///Draw sprites to screen
		void renderSprites();

		///Draw text to screen
		void renderText();

		void renderWireframes();
		
		///Get number of Sprite Components
		int getNumberOfLabels() { return mTextComponentMap.size(); }
		///Get number of Sprite Components
		int getNumberOfSprites() { return mSpriteComponentMap.size(); }
		///Get number of Transform Components
		int getNumberOfTransforms() { return mTransformComponentMap.size(); }

	private:
		RKTUtil::MemoryPool mTransformPool;
		RKTUtil::MemoryPool mLabelPool;
		RKTUtil::MemoryPool mSpritePool;
		RKTUtil::MemoryPool mColliderPool;
		RKTUtil::MemoryPool mAudioSourcePool;
		RKTUtil::MemoryPool mNativeScriptPool;

		std::map<ComponentId, TransformComponent*> mTransformComponentMap;
		std::map<ComponentId, TextComponent*> mTextComponentMap;
		std::map<ComponentId, SpriteComponent*> mSpriteComponentMap;
		std::map<ComponentId, BoxColliderComponent*> mColliderComponentMap;
		std::map<ComponentId, AudioSourceComponent*> mAudioSourceComponentMap;
		std::map<ComponentId, NativeScriptComponent*> mNativeScriptComponentMap;

		static ComponentId msNextTransformComponentId;
		static ComponentId msNextTextComponentId;
		static ComponentId msNextSpriteComponentId;
		static ComponentId msNextMaterialComponentId;
		static ComponentId msNextColliderComponentId;
		static ComponentId msNextAudioSourceComponentId;
		static ComponentId msNextNativeScriptComponentId;

	};
}
#endif // !COMPONENT_MAN_H
