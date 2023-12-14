#pragma once

#include <memory>
#include <vector>
#include <array>
#include <glm/glm.hpp>

#include "scene/SceneObject.h"
#include "scene/SceneObjectMenu.h"

#include "render/UniformBuffer.h"

class Application;

class Scene
{
public:
    void Init(Application &app);
    void SyncToRenderer(Application &app);

    void AddObject(std::unique_ptr<SceneObject> &&obj) { m_objects.emplace_back(std::move(obj)); }
    bool DrawMenu(SceneObjectMenu &menu_visitor);
    void AddLightObject() { m_lighsources.emplace_back(); };
    bool DrawLightMenu();

    void SaveState(std::ostream &f);
    void RestoreState(std::istream &f);

    std::unique_ptr<UniformBuffer> &GetRawUBO() { return m_ubo; }

private:
    std::vector<std::unique_ptr<SceneObject>> m_objects;
    std::vector<std::array<float, 8>> m_lighsources;
    std::unique_ptr<UniformBuffer> m_ubo;
};