#pragma once

#include <glm/glm.hpp>
#include <string>


class Shader
{
public:
    virtual ~Shader() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void SetInt(const std::string &name, const int value) = 0;
    virtual void SetBool(const std::string &name, const bool value) = 0;
    virtual void SetFloat(const std::string &name, const float value) = 0;
    virtual void SetFloat2(const std::string &name, const glm::vec2 &value) = 0;
    virtual void SetFloat3(const std::string &name, const glm::vec3 &value) = 0;
    virtual void SetFloat4(const std::string &name, const glm::vec4 &value) = 0;

    virtual void SetMat3(const std::string &name, const glm::mat3 &value) = 0;
    virtual void SetMat4(const std::string &name, const glm::mat4 &value) = 0;

    virtual void BindUniformBlock(const std::string &name, const int32_t index) = 0;
};
