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

#include "Component.h"
#include "RocketEngine/render/buffers/Texture.h"
#include <RocketEngine\render\Color.h>
#include <glm/vec2.hpp>

namespace RKTEngine
{
	const std::string modelFileLocation = "../RogueOne/assets/sprites/";

	struct SpriteRenderData
	{
		glm::vec3 position;
		glm::vec2 scale;
		float rotation;
	};

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

		std::string mTileName;
		int mWidth, mHeight;

		Texture2D* pSprite;			///< Reference to the model
		Color mColor;

		///Default constructor sets all values to zero
		SpriteComponentData() :
			isLoaded(false), pSprite(nullptr), mSpriteName(""), mColor(Color::white), mWidth(0), mHeight(0), mTileName("") {};

		///Constructor that takes in values for struct variables 
		//width/height set when reading in sprite
		//tileName used for id'ing atlased sprite cells
		SpriteComponentData(std::string name, std::string tileName = "", int w = 0, int h = 0, Color color = Color::white) :
			isLoaded(false), pSprite(nullptr), mSpriteName(name), mColor(color),
			mWidth(w), mHeight(h), mTileName(tileName) {};
	};

	const SpriteComponentData ZERO_SPRITE_DATA;

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

		///Access the mesh model reference
		Texture2D* getSprite();

		/**********************************************************************//**
		 * Set mesh data
		 *
		 * @param data MeshComponent struct with mesh data to store in the component
		 *************************************************************************/
		void setData(const SpriteComponentData& data);

		/**********************************************************************//**
		 * Set model refernce
		 *
		 * @param mesh A reference to a Model to be assigned to the component's mesh data
		 *************************************************************************/
		void setMesh(Texture2D* sprite) { mSpriteData.pSprite = sprite; }

		void setColor(Color color) { mSpriteData.mColor = color; }

		void setSprite(const std::string& tileName);

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
		void process(const glm::vec2& position, const glm::vec2& scale, float rotationAngle = 0);

		/**********************************************************************//**
		 * Render model
		 *
		 * @param shaderMan A reference to the shader manager to set model matrix uniform
		 *************************************************************************/
		void render();

	private:
		SpriteRenderData mRenderInfo;
		SpriteComponentData mSpriteData;
		AtlasCoordinateData mAtlasCoords;

		AtlasCoordinateData calculateAtlasCoords(int x, int y);
	};
}
#endif // !MESH_COMP_H
