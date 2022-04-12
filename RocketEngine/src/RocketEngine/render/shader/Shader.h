/********
	=========================
			 ROCKET ENGINE
	=========================
	File Created By: Walter Hill

	Rocket3D is an open source 3D game engine written using C++ & OpenGL.

	This class makes use of tutorials from Learn OpenGL
	(https://learnopengl.com)

	This code is open source under the Apache 2.0 license.
	(https://github.com/walthill/Rocket3D/blob/master/LICENSE)

	=========================
			 RK_Shader.h
	=========================
	This class encapsulates OpenGL vertex and fragment shaders. 
	Shaders are written in GLSL. 

********/

#ifndef RK_SHADER_H
#define RK_SHADER_H

#include <unordered_map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace RKTEngine
{
	typedef int UniformLocation;

	class Shader
	{
	public:
		/**********************************************************************//**
		 * Constructor that takes in shader filenames
		 *
		 * @param vertexPath Vertex shader filename
		 * @param vertexPath Fragment shader filename
		 * Takes in vertex and fragment shader filenames and initializes the shaders
		 *************************************************************************/
		Shader(std::string vertexPath, std::string fragmentPath);

		/// Initialize shader by reading GLSL code from file, compiling, and linking shaders 
		void init();

		/// Get shader code from file, store in string for use in compilation 
		std::pair<std::string, std::string> loadFromFile();

		/**********************************************************************//**
		 * Executes shader compilation, linking, and cleanup
		 *
		 * @param vertShaderCode Vertex shader code to compile
		 * @param fragShaderCode Fragment shader code to compile
		 *************************************************************************/
		void compileShaders(const char* vertShaderCode, const char* fragShaderCode);

		///Executre shader program
		void use() const;

		/**********************************************************************//**
		 * Cleanup shader initialization objects
		 *
		 * @param shaderObject Int reference to the shaderObject to destroy
		 *************************************************************************/
		void destroyShader(int shaderObject);

		/**********************************************************************//**
		 * Set boolean uniform in the shader
		 *
		 * @param name String uniform identifier
		 * @param value Boolean value to pass to the shader's uniform
		 * Set boolean uniform to the given value at the location of the given name in the shader
		 *************************************************************************/
		void setBool(const std::string& name, const bool& value) const;

		/**********************************************************************//**
		 * Set integer uniform in the shader
		 *
		 * @param name String uniform identifier
		 * @param value Integer value to pass to the shader's uniform
		 * Set integer uniform to the given value at the location of the given name in the shader
		 *************************************************************************/
		void setInt(const std::string& name, const int& value) const;

		void setIntArray(const std::string& name, int* values, uint32_t count) const;

		/**********************************************************************//**
		 * Set float uniform in the shader
		 *
		 * @param name String uniform identifier
		 * @param value Float value to pass to the shader's uniform
		 * Set float uniform to the given value at the location of the given name in the shader
		 *************************************************************************/
		void setFloat(const std::string& name, const float& value) const;

		/**********************************************************************//**
		 * Set Matrix4 uniform in the shader
		 *
		 * @param name String uniform identifier
		 * @param value Matrix4 value to pass to the shader's uniform
		 * Set Matrix4 uniform to the given value at the location of the given name in the shader
		 *************************************************************************/
		void setMat4(const std::string& name, const glm::mat4& value) const;

		/**********************************************************************//**
		 * Set Vector3 uniform in the shader
		 *
		 * @param name String uniform identifier
		 * @param value Vector3 value to pass to the shader's uniform
		 * Set Vector3 uniform to the given value at the location of the given name in the shader
		 *************************************************************************/
		void setVec3(const std::string& name, const glm::vec3& value) const;

		void setVec2(const std::string& name, const glm::vec2& value) const;

		/**********************************************************************//**
		 * Get uniform location, either from cache or an OpenGL call
		 *
		 * @param name String uniform identifier
		 * Use uniform name to check if location has been cached. If so, returns from map
		 * If not found, new location is retrieved from OpenGL and stored in the map
		 *************************************************************************/
		UniformLocation getUniformLocation(std::string name) const;

		int shaderID;

	private:
		std::string mShaderAssetPath = "assets/shaders/"; ///> Shader file path
		std::string mVertShaderPath;
		std::string mFragShaderPath;

		const int MODIFY_SINGLE_OBJECT = 1;

		///Stores unifrom locations for faster access & uniform manipulation 
		mutable std::unordered_map<std::string, UniformLocation> mUniformLocationCache;
	};
}
#endif // !RK_SHADER_H
