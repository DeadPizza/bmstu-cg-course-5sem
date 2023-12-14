#include "Project.h"

#include "application/Application.h"

Project::Project(Application &app)
    : m_scene(std::make_shared<Scene>()),
    m_camera(std::make_shared<Camera>())
{
    m_scene->Init(app);
}