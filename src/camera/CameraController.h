#pragma once

#include "Camera.h"

#include <memory>

class CameraController
{
public:
    CameraController()
        : my_cam(nullptr)
    {}

    void SetCamera(std::shared_ptr<Camera> cam) { my_cam = cam; }

    bool Update();

private:
    std::shared_ptr<Camera> my_cam;
};