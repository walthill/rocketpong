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
		ComponentManager(uint32 maxSize, ShaderManager* shaderMan);

		///Default decconstructor
		~ComponentManager();

		///Cleanup
		void clean();

		//TRANSFORM

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

		//MESH
		/**********************************************************************//**
		* Access the mesh component based on its component identifier.
		*
		* @param id Component identifier
		*************************************************************************/
		SpriteComponent* getMeshComponent(const ComponentId& id);

		/**********************************************************************//**
		* Create mesh component based on data passed in and return the component identifier.
		*
		* @param meshId mesh component identifier
		* @param data data Mesh component data
		*************************************************************************/
		ComponentId allocateMeshComponent(const ComponentId& meshId, const	SpriteComponentData& data = ZERO_MESH_DATA);

		/**********************************************************************//**
		* Remove and destroy mesh component from the collection based on the component identifier.
		*
		* @param id Component identifier
		*************************************************************************/
		void deallocateMeshComponent(const ComponentId& id);

		
		/**********************************************************************//**
		* Process components and perform calulations.
		*
		* @param elapsedTime Frame timing
		*************************************************************************/
		void update(float elapsedTime);

		///Draw model meshes to screen
		void renderSprites();

		///Get number of Mesh Components
		int getNumberOfMeshes() { return mMeshComponentMap.size(); }
		///Get number of Transform Components
		int getNumberOfTransforms() { return mTransformComponentMap.size(); }
	private:
		ShaderManager* mpShaderManagerHandle;
		ShaderKey mLightingShaderKey;

		RKTUtil::MemoryPool mTransformPool;
		RKTUtil::MemoryPool mSpritePool;

		std::map<ComponentId, TransformComponent*> mTransformComponentMap;
		std::map<ComponentId, SpriteComponent*> mMeshComponentMap;

		static ComponentId msNextTransformComponentId;
		static ComponentId msNextMeshComponentId;
		static ComponentId msNextMaterialComponentId;
	};
}
#endif // !COMPONENT_MAN_H
