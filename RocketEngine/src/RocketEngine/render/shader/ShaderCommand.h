#ifndef SHADER_COMMAND_H
#define SHADER_COMMAND_H

#include "ShaderAPI.h"

namespace RKTEngine
{
	class ShaderCommand
	{
	public:
		static void use()
		{
			spShaderAPI->use();
		}

		static void cleanup(int shaderObject)
		{
			spShaderAPI->cleanup(shaderObject);
		}

		static int compileShader(const char* vertShaderCode, const char* fragShaderCode)
		{
			return spShaderAPI->compileShader(vertShaderCode, fragShaderCode);
		}
		
		static void setBool(int layoutLocation, const bool& value)
		{
			spShaderAPI->setBool(layoutLocation, value);
		}

		static void setInt(int layoutLocation, const int& value)
		{
			spShaderAPI->setInt(layoutLocation, value);
		}

		static void setFloat(int layoutLocation, const float& value)
		{
			spShaderAPI->setFloat(layoutLocation, value);
		}
		static void setMat4(int layoutLocation, const glm::mat4& value)
		{
			spShaderAPI->setMat4(layoutLocation, value);
		}
		static void setVec3(int layoutLocation, const glm::vec3& value)
		{
			spShaderAPI->setVec3(layoutLocation, value);
		}

		static void setVec2(int layoutLocation, const glm::vec2& value)
		{
			spShaderAPI->setVec2(layoutLocation, value);
		}
		
		static int getUniformLocation(uint32 shaderID, const char* uniformName)
		{
			return spShaderAPI->getUniformLocation(shaderID, uniformName);
		}

		private:
			static ShaderAPI* spShaderAPI;
	};
}

#endif // !SHADER_API_H
