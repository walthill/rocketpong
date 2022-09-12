/********
	=========================
			 ROCKET3D
	=========================
	File Created By: Walter Hill

	Rocket3D is an open source 3D game engine written using C++ & OpenGL.

	This code is open source under the Apache 2.0 license.
	(https://github.com/walthill/Rocket3D/blob/master/LICENSE)

	=========================
		 TransformComponent.h
	=========================
	This file contains the definition for the Transform Component object.
	Inlucdes info for Transform data and manipulation of position, scale, and rotation

********/

#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "Component.h"

namespace RKTEngine
{
	struct Rotation
	{
		float angle;			///< Rotation angle
		glm::vec3 rotationAxis;	///< Axis of rotation

		///Default constructor sets all values to zero
		Rotation() : angle(0), rotationAxis(glm::vec3(0,0,0)) {}
		///Constructor that takes in values for struct variables
		Rotation(glm::vec3 axis, float a) : angle(a), rotationAxis(axis) {}

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(CEREAL_NVP(angle), CEREAL_NVP(rotationAxis));
		}

		template<class Archive>
		void load(Archive& archive)
		{
			archive(CEREAL_NVP(angle), CEREAL_NVP(rotationAxis));
		}
	};


	/***************************************************************************//**
	 * @brief 	Data used in every transform component.
	 *
	 * This struct contains data and constructors that will serve as foundational data
	 * for every gameobject's transform
	 ******************************************************************************/
	struct TransformData : ComponentData
	{
		Rotation mRotation;	///< Transform rotation data
		glm::vec2 mPosition;	///< Transform position value
		glm::vec2 mScale;		///< Transform scale value

		///Default constructor sets all values to zero
		TransformData() : mPosition(glm::vec2(0,0)), mScale(glm::vec2(1,1)), mRotation() {};

		///Constructor that takes in 2D values for struct variables
		TransformData(glm::vec2 pos, glm::vec2 scale, float angle) : mPosition(pos), mScale(glm::vec2(scale)), mRotation(glm::vec3(0.0f,0.0f, 1.0f), angle) {};

		///Constructor that takes in values for struct variables
		TransformData(glm::vec2 pos, glm::vec2 scale, glm::vec3 rotation, float angle) : mPosition(pos), mScale(scale), mRotation(rotation, angle) {};

		template<class Archive>
		void save(Archive& archive) const
		{
			archive(CEREAL_NVP(mPosition), CEREAL_NVP(mScale), CEREAL_NVP(mRotation));
		}

		template<class Archive>
		void load(Archive& archive)
		{
			archive(CEREAL_NVP(mPosition), CEREAL_NVP(mScale), CEREAL_NVP(mRotation));
		}
	};

	const TransformData ZERO_TRANSFORM_DATA;

	/***************************************************************************//**
	 * @brief 	The component for GameObject transforms.
	 *
	 * This class, derived from Component, contains data and flags needed to
	 * render and manipulate GameObjects.
	 ******************************************************************************/
	class TransformComponent : public Component
	{
		public:

			/**********************************************************************//**
			* Creates component with the given id value.
			*
			* @param id ComponentId value assigned to this instance of the Component class
			*************************************************************************/
			TransformComponent(const ComponentId& id);

			///Default deconstructor
			~TransformComponent();

			///Access the transform component data
			inline TransformData getData() { return mTransformData; }
			///Access the transform component position
			inline glm::vec2 getPosition() { return mTransformData.mPosition; };
			///Access the transform component scale
			inline glm::vec2 getScale() { return mTransformData.mScale; };
			///Access the transform component rotation data
			inline Rotation getRotation() { return mTransformData.mRotation; };
			///Check if the transform component data has changed
			inline bool hasChanged() { return mDataChanged; }

			/**********************************************************************//**
			 * Set position value
			 *
			 * @param pos New position vector
			 *************************************************************************/
			void setPosition(const glm::vec2& pos);

			void setPosition(float x, float y);

			/**********************************************************************//**
			 * Set position value
			 *
			 * @param data TransformData struct with data to store in the component
			 *************************************************************************/
			void setData(const TransformData& data);

			/**********************************************************************//**
			 * Set position value
			 *
			 * @param pos New position vector
			 *************************************************************************/
			void setScale(const glm::vec2& mScale);

			/**********************************************************************//**
			 * Set rotation value
			 *
			 * @param pos New rotation axis vector
			 * @param angle New rotation angle
			 *************************************************************************/
			void setRotation(const glm::vec3& rotationAxis, float angle);	
			void setRotation(const Rotation& rot);

			/**********************************************************************//**
			 * Set mDataChanged flag
			 *
			 * @param changed Boolean to switch mDataChanged flag.
			 *************************************************************************/
			void setHasChanged(bool changed) { mDataChanged = changed; }

			inline virtual bool isEnabled() override { return mTransformData.isEnabled; }
			inline virtual void setEnabled(bool enabled) override { mTransformData.isEnabled = enabled; }

		private:
			TransformData mTransformData;
			bool mDataChanged;
	};
}
#endif // !TRANSFORM_COMPONENT_H
