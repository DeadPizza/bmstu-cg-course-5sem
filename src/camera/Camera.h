#pragma once

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Camera
{
public:
    glm::mat4 GetViewMatrix()
    {
        auto mat = glm::toMat4(rotation);
        // return mat;
        return glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(mat[2]), glm::vec3(0.0f, 1.0f, 0.0f));
        // return glm::lookAt(position, position + glm::vec3(mat[2]), glm::vec3(mat[1]));
    }

    const glm::vec3 &GetPosition() { return position; }
    const glm::quat &GetRotation() { return rotation; }

    void SetPosition(const glm::vec3 &new_pos)
    {
        position = new_pos;
    }

    void SetRotation(const glm::quat &new_quat)
    {
        rotation = new_quat;
    }

    void Translate(const glm::vec3 &delta)
    {
        position += delta;
    }

    glm::vec3 forward() { return glm::toMat4(rotation)[2]; }

    glm::vec3 up() { return glm::toMat4(rotation)[1]; }

    glm::vec3 right() { return glm::toMat4(rotation)[0]; }

private:
    glm::vec3 position;
    glm::quat rotation;
};