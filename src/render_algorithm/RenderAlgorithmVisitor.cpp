#include "render_algorithm/RenderAlgorithmVisitor.h"

#include "imgui.h"

bool RenderAlgorithmVisitor::Visit(DebugAlgorithm &algo)
{
    return false;
}

bool RenderAlgorithmVisitor::Visit(RayTracingAlgorithm &algo)
{
    return false;
}

bool RenderAlgorithmVisitor::Visit(BPTAlgorithm &algo)
{
    bool changed_settings = false;

    changed_settings |= ImGui::InputInt("Выборка/кадр", &algo.perframe);
    changed_settings |= ImGui::InputInt("Полная выборка", &algo.max_frames);
    changed_settings |= ImGui::SliderInt("Длина пути от источника", &algo.light_path, 1, 10);
    changed_settings |= ImGui::SliderInt("Длина пути от камеры", &algo.eye_path, 1, 10);
    ImGui::Text("Прогресс: %d/%d", algo.cur_frame, algo.max_frames);

    return changed_settings;
}