#pragma once

#include "scene/Scene.h"
#include "camera/Camera.h"

#include <memory>
#include <vector>

class Application;

class Project
{
public:
    Project(Application &app);
    std::shared_ptr<Scene> GetScene() { return m_scene; }
    std::shared_ptr<Camera> GetCamera() { return m_camera; }

private:
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<Camera> m_camera;
};