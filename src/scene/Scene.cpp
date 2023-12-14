#include "scene/Scene.h"

#include "application/Application.h"
#include "scene/objects/ObjectPlane.h"
#include "SceneObjectFactory.h"
#include "imgui.h"

#define ASSIGN_VEC_3(vec, x, y, z) (vec)[0] = x; (vec)[1] = y; (vec)[2] = z
#define ASSIGN_VEC_4(vec, x, y, z, w) (vec)[0] = x; (vec)[1] = y; (vec)[2] = z; (vec)[3] = w

#define MAKE_PLANE(scene, a, b, c, d, e, f, g, h, i, j, k)\
do {\
auto &obj = m_objects.emplace_back(std::make_unique<ObjectPlane>());\
obj->m_data.object_type = 1;\
ASSIGN_VEC_4(obj->m_data.attr, a, b, c, d);\
ASSIGN_VEC_3(obj->m_data.attr + 4, e, f, g);\
ASSIGN_VEC_4(obj->m_data.attr + 8, h, i, j, k);\
} while(0)

void Scene::Init(Application &app)
{
    m_ubo.reset(app.GetRenderDevice()->GenerateUniformBuffer(sizeof(SceneObject::CommonData) * 200));

    // MAKE_PLANE(m_scene, 0.0, 1.0, 0.0, 0.0, 0., 1., 0., 1, 1, 1, 1);
    // MAKE_PLANE(m_scene, 0.0, 0.0, -1.0, 8.0, 0., 0., -1., 1, 1, 1, 1);
    // MAKE_PLANE(m_scene, 1.0, 0.0, 0.0, 0.0, 1., 0., 0., 1, 1, 1, 2);
    // // MAKE_PLANE(m_scene, 0.0, -1.0, 0.0, 5.49, 0., -1., 0., 1, 1, 1, 5.5);
    // MAKE_PLANE(m_scene, -1.0, 0.0, 0.0, 5.59, -1., 0., 0., 1, 1, 1, 3);

    // m_lighsources.emplace_back(std::array<float, 8>{3.0, 7.5, 2.5, .5});

    SyncToRenderer(app);
}

void Scene::SyncToRenderer(Application &app)
{
    std::vector<SceneObject::CommonData> tmp_vec;
    for (std::unique_ptr<SceneObject> &scene_object : m_objects)
    {
        tmp_vec.emplace_back(scene_object->GetData());
    }
    int size = tmp_vec.size();

    m_ubo->Bind(0);
    m_ubo->SubData(&size, 0, sizeof(size));
    m_ubo->SubData(&tmp_vec[0], 16, sizeof(SceneObject::CommonData) * tmp_vec.size());
    size = m_lighsources.size();
    m_ubo->SubData(&size, 16 + sizeof(SceneObject::CommonData) * 100, sizeof(size));
    m_ubo->SubData(&m_lighsources[0], 16 + sizeof(SceneObject::CommonData) * 100 + 16, sizeof(m_lighsources[0]) * size);
}

bool Scene::DrawMenu(SceneObjectMenu &menu_visitor)
{
    bool changed_flag = false;
    for (int i = 0; i < m_objects.size(); ++i)
    {
        changed_flag |= m_objects[i]->ParamsAccept(menu_visitor);
        if (menu_visitor.ShouldDeleteCur())
        {
            m_objects.erase(m_objects.begin() + i);
            --i;
            menu_visitor.ResetDeleteCur();
        }
        if (menu_visitor.ShouldCloneCur())
        {
            // m_objects.emplace_back()
            std::unique_ptr<SceneObject> tmp;
            tmp.reset(m_objects[i]->Clone());
            m_objects.emplace_back(std::move(tmp));
            menu_visitor.ResetDeleteCur();
        }
    }
    // for (std::unique_ptr<SceneObject> &scene_object : m_objects)
    // {
    //     changed_flag |= scene_object->ParamsAccept(menu_visitor);
    // }
    return changed_flag;
}

bool Scene::DrawLightMenu()
{
    bool changed = false;
    for (int i = 0; i < m_lighsources.size(); ++i)
    {
        auto cur_header = std::format("Источник света {}", i);
        if (ImGui::TreeNode(cur_header.c_str()))
        {
            float *as_float_arr = (float *)&m_lighsources[i];
            changed |= ImGui::DragFloat3("Позиция", as_float_arr);
            changed |= ImGui::DragFloat("Радиус", as_float_arr + 3);
            changed |= ImGui::DragFloat("Интенсивность", as_float_arr + 4);
            if(ImGui::Button("Удалить"))
            {
                m_lighsources.erase(m_lighsources.begin() + i);
                --i;
                changed = true;
            }
            ImGui::TreePop();
        }
    }
    return changed;
}

void Scene::SaveState(std::ostream &f)
{
    uint32_t arr_size = m_objects.size();
    f.write(reinterpret_cast<char *>(&arr_size), sizeof(uint32_t));
    for (std::unique_ptr<SceneObject> &scene_object : m_objects)
    {
        f.write(reinterpret_cast<char *>(&scene_object->m_data.object_type), sizeof(int32_t));
        f.write(reinterpret_cast<char *>(&scene_object->m_data), sizeof(scene_object->m_data));
        f.write(reinterpret_cast<char *>(&scene_object->selected_material), sizeof(scene_object->selected_material));
    }

    arr_size = m_lighsources.size();
    f.write(reinterpret_cast<char *>(&arr_size), sizeof(uint32_t));
    for (auto &light_src : m_lighsources)
    {
        f.write(reinterpret_cast<char *>(&light_src), sizeof(light_src));
    }
    // f.write(reinterpret_cast<char *>(&m_key_frames[0]), sizeof(KeyFrame) * arr_size);
}

void Scene::RestoreState(std::istream &f)
{
    uint32_t arr_size;
    f.read(reinterpret_cast<char *>(&arr_size), sizeof(uint32_t));
    m_objects.resize(arr_size);
    for (int i = 0; i < arr_size; ++i)
    {
        int32_t obj_type;
        f.read(reinterpret_cast<char *>(&obj_type), sizeof(obj_type));
        m_objects[i] = generateSceneObject(SceneObjectType(obj_type));
        f.read(reinterpret_cast<char *>(&m_objects[i]->m_data), sizeof(m_objects[i]->m_data));
        f.read(reinterpret_cast<char *>(&m_objects[i]->selected_material), sizeof(m_objects[i]->selected_material));
    }

    f.read(reinterpret_cast<char *>(&arr_size), sizeof(uint32_t));
    m_lighsources.resize(arr_size);
    for (int i = 0; i < arr_size; ++i)
    {
        f.read(reinterpret_cast<char*>(&m_lighsources[i]), sizeof(m_lighsources[i]));
    }
}