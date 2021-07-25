#include "OpenGLShader.h"
#include "RocketEngine/core/Log.h"
#include <glad/glad.h>

namespace RKTEngine
{
	void OpenGLShader::use(uint32 shaderID)
	{
		glUseProgram(shaderID);
	}

	void OpenGLShader::cleanup(int shaderObject)
	{
		glDeleteShader(shaderObject);
	}

	int OpenGLShader::compileShader(const char* vertShaderCode, const char* fragShaderCode)
	{
		// Compile shaders
		// ---------------------------------------------------------------
		unsigned int vertexShader, fragmentShader; //Shader obj

		//Debug vars
		int success;
		char infoLog[512];

		//VERTEX SHADER
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertShaderCode, NULL);
		glCompileShader(vertexShader);

		//Check if compilation successful
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			RKT_CORE_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
		}

		//FRAGMENT SHADER
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragShaderCode, NULL);
		glCompileShader(fragmentShader);

		//Check if compilation successful
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			RKT_CORE_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");
		}

		//SHADER PROGRAM
		RKT_CORE_TRACE("PERFORMING SHADER LINK");
		uint32 shaderID = glCreateProgram();
		glAttachShader(shaderID, vertexShader);
		glAttachShader(shaderID, fragmentShader);
		glLinkProgram(shaderID);

		// Check if linking successful
		glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
			RKT_CORE_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED");
		}

		//Clean up
		cleanup(vertexShader);
		cleanup(fragmentShader);

		return shaderID;
	}

	void OpenGLShader::setBool(int layoutLocation, const bool& value) const
	{
		glUniform1i(layoutLocation, (int)value);
	}

	void OpenGLShader::setInt(int layoutLocation, const int& value) const
	{
		glUniform1i(layoutLocation, (int)value);
	}
	void OpenGLShader::setFloat(int layoutLocation, const float& value) const
	{
		glUniform1f(layoutLocation, value);
	}

	void OpenGLShader::setMat4(int layoutLocation, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(layoutLocation, MODIFY_SINGLE_OBJECT, GL_FALSE, &mat[0][0]);
	}

	void OpenGLShader::setVec3(int layoutLocation, const glm::vec3& value) const
	{
		glUniform3fv(layoutLocation, MODIFY_SINGLE_OBJECT, &value[0]);
	}

	void OpenGLShader::setVec2(int layoutLocation, const glm::vec2& value) const
	{
		glUniform2fv(layoutLocation, MODIFY_SINGLE_OBJECT, &value[0]);
	}

	int OpenGLShader::getUniformLocation(uint32 shaderID, const char* name)
	{
		return glGetUniformLocation(shaderID, name);
	}
}