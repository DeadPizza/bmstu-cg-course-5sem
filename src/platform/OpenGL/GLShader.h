#pragma once

#include "render/Shader.h"
#include <glad/glad.h>

class GLShader : public Shader
{
public:
    GLShader(const std::string &vertexPath, const std::string &fragmentPath);
    ~GLShader();

    void Bind() override;
    void Unbind() override;

    void SetInt(const std::string &name, const int value) override;
    void SetBool(const std::string &name, const bool value) override;
    void SetFloat(const std::string &name, const float value) override;
    void SetFloat2(const std::string &name, const glm::vec2 &value) override;
    void SetFloat3(const std::string &name, const glm::vec3 &value) override;
    void SetFloat4(const std::string &name, const glm::vec4 &value) override;

    void SetMat3(const std::string &name, const glm::mat3 &value) override;
    void SetMat4(const std::string &name, const glm::mat4 &value) override;

    void BindUniformBlock(const std::string &name, const int32_t index) override;
private:
    void CheckCompileErrors(GLuint shader, const std::string &type);

    GLint GetLocation(const std::string &name);

    GLuint m_descriptor;
};