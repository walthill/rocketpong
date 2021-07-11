#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include "RocketEngine/render/shader/ShaderAPI.h"

namespace RKTEngine
{
	class OpenGLShader : public ShaderAPI
	{
		public:
			virtual void use() override;
			virtual void cleanup(int shaderObject) override;
			virtual int compileShader(const char* vertShaderCode, const char* fragShaderCode) override;

			virtual void setBool(int layoutLocation, const bool& value) const override;
			virtual void setInt(int layoutLocation, const int& value) const override;
			virtual void setFloat(int layoutLocation, const float& value) const override;
			virtual void setMat4(int layoutLocation, const glm::mat4& mat) const override;
			virtual void setVec3(int layoutLocation, const glm::vec3& value) const override;
			virtual void setVec2(int layoutLocation, const glm::vec2& value) const override;

			virtual int getUniformLocation(uint32 shaderID, const char* uniformName) override;
		private:
			int mShaderID;
			const int MODIFY_SINGLE_OBJECT = 1;
	};
}

#endif // !OPENGL_SHADER_H
