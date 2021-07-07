#include "Shader.h"
#include "RocketEngine/core/Log.h"
#include <fstream>
#include <sstream>
#include <glad/glad.h>

//TODO: abstract shader API calls

namespace RKTEngine
{

	Shader::Shader(std::string vertexPath, std::string fragmentPath)
	{
		mVertShaderPath = vertexPath;
		mFragShaderPath = fragmentPath;
		init();
	}

	void Shader::init()
	{
		std::pair<std::string, std::string> vertAndFragCode = loadFromFile();

		const char* vertexShaderCode = vertAndFragCode.first.c_str();
		const char* fragmentShaderCode = vertAndFragCode.second.c_str();

		compileShaders(vertexShaderCode, fragmentShaderCode);
	}

	std::pair<std::string, std::string> Shader::loadFromFile()
	{
		std::string vertexCode;
		std::string fragmentCode;

		std::ifstream vertShaderFile;
		std::ifstream fragShaderFile;

		vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			//Constructor only needs file name and not path
			std::string vFilePath, fFilePath;

			vFilePath = mSHADER_FILE_PATH + mVERTEX_PATH + mVertShaderPath;
			fFilePath = mSHADER_FILE_PATH + mFRAG_PATH + mFragShaderPath;

			vertShaderFile.open(vFilePath, std::ifstream::in);
			fragShaderFile.open(fFilePath, std::ifstream::in);

			std::stringstream vShaderStream, fShaderStream;

			//Read files into streams
			vShaderStream << vertShaderFile.rdbuf();
			fShaderStream << fragShaderFile.rdbuf();

			vertShaderFile.close();
			fragShaderFile.close();

			//Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			RKT_CORE_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
		}

		return std::pair<std::string, std::string>(vertexCode, fragmentCode);
	}

	void Shader::compileShaders(const char* vertShaderCode, const char* fragShaderCode)
	{
		// Compile shaders
		// ---------------------------------------------------------------
		RKT_CORE_TRACE("=======\tSHADER -- BUILD STARTED =======");

		unsigned int vertexShader, fragmentShader; //Shader obj

		//Debug vars
		int success;
		char infoLog[512];

		//VERTEX SHADER
		RKT_CORE_TRACE("(" + mVertShaderPath + ") -- VERTEX SHADER COMPILATION");
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
		else { RKT_CORE_INFO("COMPILATION >>>>>>> [SUCCESSFUL]"); }

		//FRAGMENT SHADER
		RKT_CORE_TRACE("(" + mFragShaderPath + ") -- FRAGMENT SHADER COMPILATION");
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
		else { RKT_CORE_INFO("COMPILATION >>>>>>> [SUCCESSFUL]"); }

		//SHADER PROGRAM
		RKT_CORE_TRACE("PERFORMING SHADER LINK");
		shaderID = glCreateProgram();
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
		else
		{
			RKT_CORE_INFO("LINKING >>>>>>> [SUCCESSFUL]");
		}

		//Clean up
		destroyShader(vertexShader);
		destroyShader(fragmentShader);
	}

	void Shader::use()
	{
		glUseProgram(shaderID);
	}

	void Shader::destroyShader(int shaderObject)
	{
		glDeleteShader(shaderObject);
	}

	void Shader::setBool(const std::string& name, const bool& value) const
	{
		UniformLocation location = getUniformLocation(name);
		glUniform1i(location, (int)value);
	}

	void Shader::setInt(const std::string& name, const int& value) const
	{
		UniformLocation location = getUniformLocation(name);
		glUniform1i(location, value);
	}

	void Shader::setFloat(const std::string& name, const float& value) const
	{
		UniformLocation location = getUniformLocation(name);
		glUniform1f(location, value);
	}

	void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
	{
		UniformLocation location = getUniformLocation(name);
		glUniformMatrix4fv(location, MODIFY_SINGLE_OBJECT, GL_TRUE, &mat[0][0]);
	}

	void Shader::setVec3(const std::string& name, const glm::vec3& value)
	{
		UniformLocation location = getUniformLocation(name);
		glUniform3fv(location, MODIFY_SINGLE_OBJECT, &value[0]);
	}

	void Shader::setVec2(const std::string& name, const glm::vec2& value)
	{
		UniformLocation location = getUniformLocation(name);
		glUniform2fv(location, MODIFY_SINGLE_OBJECT, &value[0]);
	}

	UniformLocation Shader::getUniformLocation(std::string name) const
	{
		//Find existing uniform, return its location value
		auto locationIndex = mUniformLocationCache.find(name);
		if (locationIndex != mUniformLocationCache.end())
			return locationIndex->second;

		//Get location from OpenGL, store in cache, and return
		UniformLocation location = glGetUniformLocation(shaderID, name.c_str());
		mUniformLocationCache[name] = location;
		return location;
	}
}