/********
	=========================
			 ROCKET3D
	=========================
	File Created By: Walter Hill

	Rocket3D is an open source 3D game engine written using C++ & OpenGL.

	This code is open source under the Apache 2.0 license.
	(https://github.com/walthill/Rocket3D/blob/master/LICENSE)

	=========================
		 MeshComponent.h
	=========================
	This file contains the definition for the Mesh Component object. 
	Inlucdes info for Mesh data and matrix uniforms

********/

#ifndef MESH_COMP_H
#define MESH_COMP_H

#include <glm/mat4x4.hpp>
#include "Component.h"
#include "RocketEngine/render/buffers/Texture.h"
#include <RocketEngine\render\shader\Shader.h>
#include <RocketEngine\render\Color.h>

namespace RKTEngine
{

	const std::string modelFileLocation = "../RogueOne/assets/sprites/";

	/***************************************************************************//**
	 * @brief 	Data used in every mesh component.
	 *
	 * This struct contains data and constructors that will serve as foundational data
	 * for every mesh
	 ******************************************************************************/
	struct SpriteComponentData
	{
		bool isLoaded;			///< Flag to set when model is loaded
		std::string mSpriteName;  ///< Name of model to load
		std::string shaderKey;	///< Shader identifier

		int instanceCount;
		glm::mat4* instanceMatrices;

		Texture2D* sprite;			///< Reference to the model
		Shader* shader;		///< Reference to the mesh's shader
		Color mColor;

		///Default constructor sets all values to zero
		SpriteComponentData() :
			isLoaded(false), sprite(nullptr), shader(nullptr), mSpriteName(""), mColor(Color::white), shaderKey(""), instanceCount(0), instanceMatrices(nullptr) {};

		///Constructor that takes in values for struct variables
		SpriteComponentData(std::string name, std::string shaderKey, Shader* s, Color color = Color::white, int instanceAmount = 1, glm::mat4* matrices = nullptr) :
			isLoaded(false), sprite(nullptr), shader(s), shaderKey(shaderKey), mSpriteName(name), mColor(color), instanceCount(instanceAmount), instanceMatrices(matrices) {};
	};

	const SpriteComponentData ZERO_MESH_DATA;

	/***************************************************************************//**
	 * @brief 	The component for Meshes.
	 *
	 * This class, derived from Component, contains data and uniform names
	 * needed to render 3d models.
	 ******************************************************************************/
	class SpriteComponent : public Component
	{
	public:

		/**********************************************************************//**
		* Creates component with the given id value.
		*
		* @param id ComponentId value assigned to this instance of the Component class
		*************************************************************************/
		SpriteComponent(const ComponentId& id);

		///Default deconstructor
		~SpriteComponent();

		///Load model from file
		void load();
		///Cleanup
		void cleanup();

		///Access the mesh component data
		SpriteComponentData* getData() { return &mSpriteData; }
		///Access the mesh shader reference
		Shader* getShader() { return mSpriteData.shader; }
		///Access the mesh model reference
		Texture2D* getSprite();

		/**********************************************************************//**
		 * Set mesh data
		 *
		 * @param data MeshComponent struct with mesh data to store in the component
		 *************************************************************************/
		void setData(const SpriteComponentData& data) { mSpriteData = data; }

		/**********************************************************************//**
		 * Set shader refernce
		 *
		 * @param shader A reference to a RK_Shader to be assign to the component's mesh data
		 *************************************************************************/
		void setShader(Shader* shader) { mSpriteData.shader = shader; }

		/**********************************************************************//**
		 * Set model refernce
		 *
		 * @param mesh A reference to a Model to be assigned to the component's mesh data
		 *************************************************************************/
		void setMesh(Texture2D* sprite) { mSpriteData.sprite = sprite; }

		void setColor(Color color) { mSpriteData.mColor = color; }

		/**********************************************************************//**
		 * Set model visibility
		 *
		 * @param show A boolean to set the isEnabled flag to show or hide the model when rendering
		 *************************************************************************/
		void setSpriteVisible(bool show);

		/**********************************************************************//**
		 * Set model visibility
		 *
		 * @param position			The world position of the model
		 * @param scale				The scale of the model
		 * @param rotationAxis		The rotational axis of the model
		 * @param rotationAngle		The rotation angle of the model
		 *************************************************************************/
		void process(glm::vec2 position = glm::vec2(0,0), glm::vec2 scale = glm::vec2(1,1), float rotationAngle = 0);

		/**********************************************************************//**
		 * Render model
		 *
		 * @param shaderMan A reference to the shader manager to set model matrix uniform
		 *************************************************************************/
		void render();

	private:
		const std::string mMODEL_MATRIX_ID = "model";
		const std::string mSPRITE_COLOR_ID = "spriteColor";
		glm::mat4 mModelMatrix = glm::mat4();
		SpriteComponentData mSpriteData;

		void attatchSpriteData();
	};
}
#endif // !MESH_COMP_H
