/********
	=========================
			 ROCKET ENGINE
	=========================
	File Created By: Walter Hill

	Rocket3D is an open source 3D game engine written using C++ & OpenGL.

	This code is open source under the Apache 2.0 license.
	(https://github.com/walthill/Rocket3D/blob/master/LICENSE)

	=========================
			 ShaderManager.cpp
	=========================

********/

#include "ShaderManager.h"
#include "Shader.h"
#include "RocketEngine/core/Log.h"

namespace RKTEngine
{
	std::map<ShaderKey, Shader*> ShaderManager::mShaderList;

	ShaderManager::ShaderManager()
	{
		mpCurrentShader = nullptr;
		mShaderList = {};
	}

	ShaderManager::~ShaderManager()
	{
		clean();
	}

	void ShaderManager::clean()
	{
		std::map<ShaderKey, Shader*>::iterator iter;

		for (iter = mShaderList.begin(); iter != mShaderList.end(); ++iter)
		{
			delete iter->second;
		}
	}

	void ShaderManager::buildShaders()
	{
		std::map<ShaderKey, Shader*>::iterator iter = mShaderList.begin();

		if (iter != mShaderList.end())
		{
			iter->second->init();
		}
	}

	void ShaderManager::useShaderByKey(const ShaderKey& key)
	{
		if (key != mShaderInUse)
		{
			const auto& iter = mShaderList.find(key);

			if (iter != mShaderList.end())
			{
				mpCurrentShader = iter->second;
				mpCurrentShader->use();
				mShaderInUse = key;
			}
		}
	}

	void ShaderManager::setShaderInt(const std::string& intName, int value)
	{
		mpCurrentShader->setInt(intName, value);
	}

	void ShaderManager::setShaderBool(const std::string& boolName, bool value)
	{
		mpCurrentShader->setBool(boolName, value);
	}

	void ShaderManager::setShaderFloat(const std::string& floatName, float value)
	{
		mpCurrentShader->setFloat(floatName, value);
	}


	void ShaderManager::setShaderMat4(const std::string& matrixName, const glm::mat4& mat)
	{
		mpCurrentShader->setMat4(matrixName, mat);
	}

	void ShaderManager::setShaderVec3(const std::string& vecName, const glm::vec3& vec)
	{
		mpCurrentShader->setVec3(vecName, vec);
	}

	void ShaderManager::setShaderVec3(const std::string& vecName, float x, float y, float z)
	{
		mpCurrentShader->setVec3(vecName, glm::vec3(x, y, z));
	}


	void ShaderManager::setShaderVec2(const std::string& vecName, float x, float y)
	{
		mpCurrentShader->setVec2(vecName, glm::vec2(x, y));
	}

	void ShaderManager::setShaderVec2(const std::string& vecName, const glm::vec2& vec)
	{
		mpCurrentShader->setVec2(vecName, vec);
	}


	Shader* ShaderManager::getShaderByKey(const ShaderKey& key)
	{
		const auto& iter = mShaderList.find(key);

		if (iter != mShaderList.end())
		{
			return iter->second;
		}
		else
		{
			RKT_CORE_ERROR("RK_Shader::Shader name invalid: " + key);
		}

		return nullptr;
	}
}