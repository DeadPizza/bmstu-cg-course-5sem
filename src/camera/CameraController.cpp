#include "camera/CameraController.h"
#include "managers/InputManager.h"

bool CameraController::Update()
{
    static float rotX, rotY;
    rotX -= InputManager::GetMouseHorizontal();
    rotY -= InputManager::GetMouseVertical();
    
    if (rotY > 89)
    {
        rotY = 89;
    }
    else if (rotY < -89)
    {
        rotY = -89;
    }

    glm::vec3 dir = my_cam->right() * InputManager::GetAxisHorizontal() +
        my_cam->forward() * -InputManager::GetAxisVertical();

    bool changed = glm::length(dir) > 0 || InputManager::GetMouseHorizontal() != 0 || InputManager::GetMouseVertical() != 0;

    dir *= 0.1;

    my_cam->Translate(dir);
    my_cam->SetRotation(glm::quat(glm::vec3(glm::radians(rotY), glm::radians(rotX), 0.0f)));
    return changed;
}