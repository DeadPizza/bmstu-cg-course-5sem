#include "scene/SceneObjectMenu.h"

#include <imgui.h>
#include <format>
#include <string>

static bool common_material(float *target, int *material)
{
    bool changed = false;
    static const char *possible_materials[] = {
        "Непрозрачный",
        "Прозрачный",
        "Зеркальный"
    };

    if (ImGui::Combo("Материал", material, possible_materials, 3))
    {
        changed = true;
        switch (*material)
        {
            case 0:
            {
                *target = 0;
            } break;

            case 1:
            {
                *target = 1;
            } break;

            case 2:
            {
                *target = -3;
            } break;
        }
    }

    switch (*material)
    {
        case 0:
        {
            // *target = 0;
        } break;

        case 1:
        {
            changed |= ImGui::DragFloat("Показатель преломления", target, 0.1, 1, 5);
        } break;

        case 2:
        {
            // *target = -2;
        } break;
    }
    return changed;
};

bool SceneObjectMenu::ParamsVisit(ObjectPlane &obj)
{
    // extra_1 = normal flip flag;
    bool changed_flag = false;
    std::string cur_header = std::format("Параметры плоскости-{}", m_cur_object++);
    if (ImGui::TreeNode(cur_header.c_str()))
    {
        changed_flag |= ImGui::DragFloat4("Коэффициенты уравнения", obj.m_data.attr, 0.25);
        // changed_flag |= ImGui::DragFloat4("Инверсия нормали", obj.m_data.attr + 4);
        if (ImGui::Checkbox("Инвертировать нормаль", &obj.m_data.attr_flag))
        {
            changed_flag = true;
            obj.m_data.extra_attr[0] = (obj.m_data.attr_flag) ? -1 : 1;
        }
        changed_flag |= ImGui::ColorEdit3("Цвет", obj.m_data.attr + 8);
        // changed_flag |= ImGui::InputFloat("Материал", obj.m_data.attr + 11);
        changed_flag |= common_material(obj.m_data.attr + 11, &obj.selected_material);
        changed_flag |= (delete_cur = ImGui::Button("Удалить"));
        ImGui::SameLine();
        changed_flag |= (clone_cur = ImGui::Button("Клонировать"));
        ImGui::TreePop();
    }
    return changed_flag;
}

bool SceneObjectMenu::ParamsVisit(ObjectSphere &obj)
{
    bool changed_flag = false;
    std::string cur_header = std::format("Параметры сферы-{}", m_cur_object++);
    if (ImGui::TreeNode(cur_header.c_str()))
    {
        changed_flag |= ImGui::DragFloat3("Позиция", obj.m_data.attr, 0.25);
        changed_flag |= ImGui::ColorEdit3("Цвет", obj.m_data.attr + 4);
        // changed_flag |= ImGui::InputFloat("Материал", obj.m_data.attr + 7);
        changed_flag |= common_material(obj.m_data.attr + 7, &obj.selected_material);
        changed_flag |= ImGui::DragFloat("Радиус", obj.m_data.attr + 3, 0.25, 0, 1000);
        changed_flag |= (delete_cur = ImGui::Button("Удалить"));
        ImGui::SameLine();
        changed_flag |= (clone_cur = ImGui::Button("Клонировать"));
        ImGui::TreePop();
    }
    return changed_flag;
}

bool SceneObjectMenu::ParamsVisit(ObjectBox &obj)
{
    bool changed_flag = false;
    std::string cur_header = std::format("Параметры параллелепипеда-{}", m_cur_object++);
    if (ImGui::TreeNode(cur_header.c_str()))
    {
        changed_flag |= ImGui::DragFloat3("Размеры", obj.m_data.attr, 0.25, 0, 1000);
        changed_flag |= ImGui::ColorEdit3("Цвет", obj.m_data.attr + 4);
        // changed_flag |= ImGui::InputFloat("Материал", obj.m_data.attr + 7);
        changed_flag |= common_material(obj.m_data.attr + 7, &obj.selected_material);
        changed_flag |= ImGui::DragFloat3("Позиция", obj.m_data.attr + 8, 0.25);
        // changed_flag |= ImGui::DragFloat("Радиус", obj.m_data.attr + 3, 0.25);
        changed_flag |= (delete_cur = ImGui::Button("Удалить"));
        ImGui::SameLine();
        changed_flag |= (clone_cur = ImGui::Button("Клонировать"));
        ImGui::TreePop();
    }
    return changed_flag;
}

bool SceneObjectMenu::ParamsVisit(ObjectHexPrism &obj)
{
    bool changed_flag = false;
    std::string cur_header = std::format("Параметры призмы-{}", m_cur_object++);
    if (ImGui::TreeNode(cur_header.c_str()))
    {
        changed_flag |= ImGui::DragFloat("Радиус", obj.m_data.attr, 0.25, 0, 1000);
        changed_flag |= ImGui::DragFloat("Высота", obj.m_data.attr + 1, 0.25, 0, 1000);
        changed_flag |= ImGui::ColorEdit3("Цвет", obj.m_data.attr + 4);
        // changed_flag |= ImGui::InputFloat("Материал", obj.m_data.attr + 7);
        changed_flag |= common_material(obj.m_data.attr + 7, &obj.selected_material);
        changed_flag |= ImGui::DragFloat3("Позиция", obj.m_data.attr + 8, 0.25);
        // changed_flag |= ImGui::DragFloat("Радиус", obj.m_data.attr + 3, 0.25);
        changed_flag |= (delete_cur = ImGui::Button("Удалить"));
        ImGui::SameLine();
        changed_flag |= (clone_cur = ImGui::Button("Клонировать"));
        ImGui::TreePop();
    }
    return changed_flag;
}

bool SceneObjectMenu::ParamsVisit(ObjectTriangle &obj)
{
    bool changed_flag = false;
    std::string cur_header = std::format("Параметры треугольника-{}", m_cur_object++);
    if (ImGui::TreeNode(cur_header.c_str()))
    {
        changed_flag |= ImGui::DragFloat3("Вершина-1", obj.m_data.attr);
        changed_flag |= ImGui::DragFloat3("Вершина-2", obj.m_data.attr + 4);
        changed_flag |= ImGui::DragFloat3("Вершина-3", obj.m_data.attr + 8);
        float tmp_color[3] = { obj.m_data.attr[3], obj.m_data.attr[7], obj.m_data.attr[11] };
        changed_flag |= ImGui::ColorEdit3("Цвет", tmp_color);
        changed_flag |= common_material(obj.m_data.extra_attr + 0, &obj.selected_material);
        obj.m_data.attr[3] = tmp_color[0];
        obj.m_data.attr[7] = tmp_color[1];
        obj.m_data.attr[11] = tmp_color[2];
        changed_flag |= (delete_cur = ImGui::Button("Удалить"));
        ImGui::SameLine();
        changed_flag |= (clone_cur = ImGui::Button("Клонировать"));
        ImGui::TreePop();
    }
    return changed_flag;
}

bool SceneObjectMenu::ParamsVisit(ObjectThorus &obj)
{
    bool changed_flag = false;
    std::string cur_header = std::format("Параметры тора-{}", m_cur_object++);
    if (ImGui::TreeNode(cur_header.c_str()))
    {
        changed_flag |= ImGui::DragFloat("Радиус вращения", obj.m_data.attr, 0.25, 0, 1000);
        changed_flag |= ImGui::DragFloat("Радиус образующей", obj.m_data.attr + 1, 0.25, 0, 1000);
        changed_flag |= ImGui::ColorEdit3("Цвет", obj.m_data.attr + 4);
        // changed_flag |= ImGui::InputFloat("Материал", obj.m_data.attr + 7);
        changed_flag |= common_material(obj.m_data.attr + 7, &obj.selected_material);
        changed_flag |= ImGui::DragFloat3("Позиция", obj.m_data.attr + 8, 0.25);
        // changed_flag |= ImGui::DragFloat("Радиус", obj.m_data.attr + 3, 0.25);
        changed_flag |= (delete_cur = ImGui::Button("Удалить"));
        ImGui::SameLine();
        changed_flag |= (clone_cur = ImGui::Button("Клонировать"));
        ImGui::TreePop();
    }
    return changed_flag;
}