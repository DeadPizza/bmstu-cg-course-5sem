#include "ui/UIAnimator.h"
#include "application/Application.h"
#include "imgui.h"

#include <format>
#include <glm/glm.hpp>
#include <glm/gtx/spline.hpp>

#include <iostream>

template <typename T>
T my_lerp(const T &a, const T &b, float t)
{
    return a * (1 - t) + b * t;
}

void UIAnimator::Init(Application &app)
{
}

bool UIAnimator::Update(Application &app)
{
    bool changed = false;

    static bool active = false;
    static int framenum = 0;
    static float t = 0;

    ImGui::Begin("Анимация");
    for (int i = 0; i < m_key_frames.size(); ++i)
    {
        if (ImGui::TreeNode(std::format("Ключевой кадр {}", i).c_str()))
        {
            ImGui::Text("Позиция: %f, %f, %f",
                m_key_frames[i].position.x,
                m_key_frames[i].position.y,
                m_key_frames[i].position.z);

            ImGui::Text("Вращение: %f, %f, %f, %f",
                m_key_frames[i].rotation.x,
                m_key_frames[i].rotation.y,
                m_key_frames[i].rotation.z,
                m_key_frames[i].rotation.w);

            if (ImGui::Button("Удалить кадр"))
            {
                m_key_frames.erase(m_key_frames.begin() + i);
                --i;
            }
            ImGui::TreePop();
        }
    }

    if (ImGui::Button("Добавить"))
    {
        changed = true;
        m_key_frames.emplace_back(KeyFrame{
            .position = app.GetProject()->GetCamera()->GetPosition(),
            .rotation = app.GetProject()->GetCamera()->GetRotation()
            });
    }

    static int framerate = 60;

    if (ImGui::TreeNode("Экспорт"))
    {
        ImGui::DragInt("Фреймрейт", &framerate, 1, 24, 60);
        if (ImGui::Button("Анимировать"))
        {
            changed = true;
            active = m_key_frames.size() >= 2;
            app.BeginExport(framerate);
            begin_time = glfwGetTime();
            framenum = 0;
        }
        ImGui::TreePop();
    }


    if (active)
    {
        float current_frame = framenum * framerate + t * framerate;
        float total_frames = (m_key_frames.size() - 1) * framerate;
        float ratio = current_frame / total_frames;
        float time_elapsed = glfwGetTime() - begin_time;
        float approx_time = time_elapsed / ratio;
        float time_rest = approx_time - time_elapsed;

        int hours = time_rest / 3600;
        int minutes = int(time_rest / 60) % 60;
        int seconds = int(time_rest) % 60;

        ImGui::ProgressBar(ratio);
        ImGui::Text("Осталось: %d:%d:%d", hours, minutes, seconds);
    }

    ImGui::End();
    if (active && app.GetRenderAlgo()->GetRenderState(app) == 1)
    {
        t += 1. / float(framerate);
        if (t > 1)
        {
            t = 0;
            ++framenum;
            if (framenum + 1 >= m_key_frames.size())
            {
                active = false;
                t = 0;
                framenum = 0;
                app.EndExport();
                return false;
            }
        }
        // glReadPixels(0, 0, 1280, 720, GL_RGB, GL_UNSIGNED_BYTE, )

        auto cam = app.GetProject()->GetCamera();

        int ind1 = glm::clamp<int>(framenum - 1, 0, m_key_frames.size() - 1);
        int ind2 = glm::clamp<int>(framenum + 0, 0, m_key_frames.size() - 1);
        int ind3 = glm::clamp<int>(framenum + 1, 0, m_key_frames.size() - 1);
        int ind4 = glm::clamp<int>(framenum + 2, 0, m_key_frames.size() - 1);

        cam->SetPosition(glm::catmullRom(
            m_key_frames[ind1].position,
            m_key_frames[ind2].position,
            m_key_frames[ind3].position,
            m_key_frames[ind4].position,
            t
        ));

        cam->SetRotation(glm::catmullRom(
            m_key_frames[ind1].rotation,
            m_key_frames[ind2].rotation,
            m_key_frames[ind3].rotation,
            m_key_frames[ind4].rotation,
            t
        ));
        // cam->SetPosition(my_lerp(m_key_frames[framenum].position, m_key_frames[framenum + 1].position, t));
        // cam->SetRotation(my_lerp(m_key_frames[framenum].rotation, m_key_frames[framenum + 1].rotation, t));
        // cam->SetPosition(m_key_frames[framenum + 1].position * t + m_key_frames[framenum].position * (1 - t));
        // cam->SetRotation(m_key_frames[framenum + 1].rotation * t + m_key_frames[framenum].rotation * (1 - t));
        changed = true;
    }
    return changed;
}

void UIAnimator::SaveState(std::ostream &f)
{
    uint32_t arr_size = m_key_frames.size();
    f.write(reinterpret_cast<char *>(&arr_size), sizeof(uint32_t));
    f.write(reinterpret_cast<char *>(&m_key_frames[0]), sizeof(KeyFrame) * arr_size);
}

void UIAnimator::RestoreState(std::istream &f)
{
    uint32_t arr_size;
    f.read(reinterpret_cast<char *>(&arr_size), sizeof(uint32_t));
    m_key_frames.resize(arr_size);
    f.read(reinterpret_cast<char *>(&m_key_frames[0]), sizeof(KeyFrame) * arr_size);
}
