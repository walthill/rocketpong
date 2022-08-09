/********
	=========================
			 ROCKET3D
	=========================
	File Created By: Walter Hill

	Rocket3D is an open source 3D game engine written using C++ & OpenGL.

	This code is open source under the Apache 2.0 license.
	(https://github.com/walthill/Rocket3D/blob/master/LICENSE)

	=========================
		   Component.h
	=========================
	The base class for all components. 
	Contains an id variable for use in the component mananger and 
	an enabled flag to be used by derived class

********/

#ifndef COMPONENT_H
#define COMPONENT_H

#include <limits>
#include <glm/glm.hpp>
#include <cereal/cereal.hpp>
#include <RKTUtils/Trackable.h>
#include "RocketEngine/Defines.h"

namespace glm
{
	template<class Archive> void serialize(Archive& archive, glm::vec2& v) { archive(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y)); }
	template<class Archive> void serialize(Archive& archive, glm::vec3& v) { archive(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y), cereal::make_nvp("z", v.z)); }
	template<class Archive> void serialize(Archive& archive, glm::vec4& v) { archive(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y), cereal::make_nvp("z", v.z), cereal::make_nvp("w", v.w)); }
	template<class Archive> void serialize(Archive& archive, glm::ivec2& v) { archive(v.x, v.y); }
	template<class Archive> void serialize(Archive& archive, glm::ivec3& v) { archive(v.x, v.y, v.z); }
	template<class Archive> void serialize(Archive& archive, glm::ivec4& v) { archive(v.x, v.y, v.z, v.w); }
	template<class Archive> void serialize(Archive& archive, glm::uvec2& v) { archive(v.x, v.y); }
	template<class Archive> void serialize(Archive& archive, glm::uvec3& v) { archive(v.x, v.y, v.z); }
	template<class Archive> void serialize(Archive& archive, glm::uvec4& v) { archive(v.x, v.y, v.z, v.w); }
	template<class Archive> void serialize(Archive& archive, glm::dvec2& v) { archive(v.x, v.y); }
	template<class Archive> void serialize(Archive& archive, glm::dvec3& v) { archive(v.x, v.y, v.z); }
	template<class Archive> void serialize(Archive& archive, glm::dvec4& v) { archive(v.x, v.y, v.z, v.w); }

	// glm matrices serialization
	template<class Archive> void serialize(Archive& archive, glm::mat2& m) { archive(m[0], m[1]); }
	template<class Archive> void serialize(Archive& archive, glm::dmat2& m) { archive(m[0], m[1]); }
	template<class Archive> void serialize(Archive& archive, glm::mat3& m) { archive(m[0], m[1], m[2]); }
	template<class Archive> void serialize(Archive& archive, glm::mat4& m) { archive(m[0], m[1], m[2], m[3]); }
	template<class Archive> void serialize(Archive& archive, glm::dmat4& m) { archive(m[0], m[1], m[2], m[3]); }

	template<class Archive> void serialize(Archive& archive, glm::quat& q) { archive(q.x, q.y, q.z, q.w); }
	template<class Archive> void serialize(Archive& archive, glm::dquat& q) { archive(q.x, q.y, q.z, q.w); }
}

namespace RKTEngine
{

	const uint32 INVALID_COMPONENT_ID = UINT_MAX;

	/***************************************************************************//**
	 * @brief 	The base class for all component classes.
	 *
	 * This base class contains an id variable for use in the component mananger and
	 * an enabled flag to be used by derived class.
	 * All derived classes must construct this class by passing an id value
	 ******************************************************************************/
	class Component : public RKTUtil::Trackable
	{
	public:

		/**********************************************************************//**
		 * Creates Component object with the given id value.
		 *
		 * @param id ComponentId value assigned to this instance of the Component class
		 *************************************************************************/
		Component(const ComponentId& id) : mIsEnabled(true) { mId = id; };

		/*
			"Virtual destructors are useful when you might potentially delete
			an instance of a derived class through a pointer to base class"
			https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
		*/

		/***************************************************************************//**
		 * @brief 	Virtual destructor to be overrode in derived class
		 *
		 * 	Virtual destructors are useful when you might potentially delete
		 *	an instance of a derived class through a pointer to base class.
		 ******************************************************************************/
		virtual ~Component() {};

		///Access the id of the component
		inline ComponentId getId() { return mId; }
	protected:
		ComponentId mId;
	protected:
		bool mIsEnabled;
	};
}

#endif // !COMPONENT_H
