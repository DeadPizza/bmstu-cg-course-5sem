#include "application/Application.h"
#include "ui/UIAlgorithmProperties.h"

#include "imgui.h"

#include "render_algorithm/RayTracingAlgorithm.h"
#include "render_algorithm/BPTAlgorithm.h"
#include "render_algorithm/DebugAlgorithm.h"
#include "render_algorithm/RenderAlgorithmVisitor.h"

void UIAlgorithmProperties::Init(Application &app)
{
    m_cur_algo = 0;

    m_render_algos.emplace_back(std::make_shared<DebugAlgorithm>());
    m_render_algos.emplace_back(std::make_shared<RayTracingAlgorithm>());
    m_render_algos.emplace_back(std::make_shared<BPTAlgorithm>());

    for(auto &algo : m_render_algos)
    {
        algo->Init(app);
    }

    app.SetRenderAlgo(m_render_algos[m_cur_algo]);
}

bool UIAlgorithmProperties::Update(Application &app)
{
    ImGui::Begin("Настройки рендера");
    auto &io = ImGui::GetIO();

    bool changed_settings = false;

    ImGui::Text("Производительность %.3f мс/кадр (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    static bool unlock_framerate = false;
    if (ImGui::Checkbox("Разблокировать FPS", &unlock_framerate))
    {
        changed_settings = true;
        if (unlock_framerate)
            glfwSwapInterval(0);
        else
            glfwSwapInterval(-1);
    }

    static const char *available_algos[] = { "Отладка", "Трассировка лучей", "Двунаправленная трассировка пути" };
    if (ImGui::Combo("Активный алгоритм", &m_cur_algo, available_algos, 3))
    {
        changed_settings = true;
        app.SetRenderAlgo(m_render_algos[m_cur_algo]);
    }

    RenderAlgorithmVisitor visitor;
    changed_settings |= m_render_algos[m_cur_algo]->Accept(visitor);

    ImGui::End();
    return changed_settings;
}

