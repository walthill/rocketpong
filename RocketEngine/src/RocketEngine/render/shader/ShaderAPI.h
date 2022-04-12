#ifndef SHADER_API_H
#define SHADER_API_H

#include "RocketEngine/Defines.h"
#include <glm/mat4x4.hpp>

namespace RKTEngine
{
	class ShaderAPI
	{
		public:
			virtual void use(uint32 shaderID) OVERRIDE_REQUIRED;
			virtual void cleanup(int shaderObject) OVERRIDE_REQUIRED;
			virtual int compileShader(const char* vertShaderCode, const char* fragShaderCode) OVERRIDE_REQUIRED;

			virtual void setBool(int layoutLocation, const bool& value) const OVERRIDE_REQUIRED;
			virtual void setInt(int layoutLocation, const int& value) const OVERRIDE_REQUIRED;
			virtual void setIntArray(int layoutLocation, int* values, uint32 count) const OVERRIDE_REQUIRED;
			virtual void setFloat(int layoutLocation, const float& value) const OVERRIDE_REQUIRED;
			virtual void setMat4(int layoutLocation, const glm::mat4& mat) const OVERRIDE_REQUIRED;
			virtual void setVec3(int layoutLocation, const glm::vec3& value) const OVERRIDE_REQUIRED;
			virtual void setVec2(int layoutLocation, const glm::vec2& value) const OVERRIDE_REQUIRED;

			virtual int getUniformLocation(uint32 shaderID, const char* uniformName) OVERRIDE_REQUIRED;
	};
}

#endif // !SHADER_API_H
