#include "Shader.h"
#include "ShaderCommand.h"
#include "RocketEngine/core/Log.h"
#include <fstream>
#include <sstream>

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

			vFilePath = mShaderAssetPath + mVertShaderPath;
			fFilePath = mShaderAssetPath + mFragShaderPath;

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
		int index = mVertShaderPath.find_first_of(".");
		RKT_CORE_TRACE(mVertShaderPath.substr(0, index) + " SHADER COMPLIATION");

		shaderID = ShaderCommand::compileShader(vertShaderCode, fragShaderCode);
	}

	void Shader::use() const
	{
		ShaderCommand::use(shaderID);
	}

	void Shader::destroyShader(int shaderObject)
	{
		ShaderCommand::cleanup(shaderObject);
	}

	void Shader::setBool(const std::string& name, const bool& value) const
	{
		UniformLocation location = getUniformLocation(name);
		ShaderCommand::setBool(location, value);
	}

	void Shader::setInt(const std::string& name, const int& value) const
	{
		UniformLocation location = getUniformLocation(name);
		ShaderCommand::setInt(location, value);
	}

	void Shader::setIntArray(const std::string& name, int* values, uint32_t count) const
	{
		UniformLocation location = getUniformLocation(name);
		ShaderCommand::setIntArray(location, values, count);
	}

	void Shader::setFloat(const std::string& name, const float& value) const
	{
		UniformLocation location = getUniformLocation(name);
		ShaderCommand::setFloat(location, value);
	}

	void Shader::setMat4(const std::string& name, const glm::mat4& value) const
	{
		UniformLocation location = getUniformLocation(name);
		ShaderCommand::setMat4(location, value);
	}

	void Shader::setVec3(const std::string& name, const glm::vec3& value) const
	{
		UniformLocation location = getUniformLocation(name);
		ShaderCommand::setVec3(location, value);
	}

	void Shader::setVec2(const std::string& name, const glm::vec2& value) const
	{
		UniformLocation location = getUniformLocation(name);
		ShaderCommand::setVec2(location, value);
	}

	UniformLocation Shader::getUniformLocation(std::string name) const
	{
		//Find existing uniform, return its location value
		const auto& locationIndex = mUniformLocationCache.find(name);
		if (locationIndex != mUniformLocationCache.end())
			return locationIndex->second;

		//Get location from OpenGL, store in cache, and return
		UniformLocation location = ShaderCommand::getUniformLocation(shaderID, name.c_str());
		mUniformLocationCache[name] = location;
		return location;
	}
}