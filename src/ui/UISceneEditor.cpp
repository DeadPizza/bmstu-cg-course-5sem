#include "ui/UISceneEditor.h"
#include "scene/SceneObjectFactory.h"
#include "application/Application.h"

#include <imgui.h>

void UISceneEditor::Init(Application &app)
{
}

bool UISceneEditor::Update(Application &app)
{
    bool changed = false;

    std::shared_ptr<Scene> scene = app.GetProject()->GetScene();
    m_menu_visitor.Reset();
    ImGui::Begin("Редактор сцены");
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Объекты"))
        {
            // ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
            changed |= scene->DrawMenu(m_menu_visitor);

            ImGui::Combo("Тип объекта", &m_cur_obj, SceneObjectTypeLiterals, 6); ImGui::SameLine();
            if (ImGui::Button("Добавить"))
            {
                changed = true;
                scene->AddObject(generateSceneObject(SceneObjectType(m_cur_obj)));
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Источники"))
        {
            changed |= scene->DrawLightMenu();
            if (ImGui::Button("Добавить"))
            {
                changed = true;
                scene->AddLightObject();
            }
            // ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
    return changed;
}

