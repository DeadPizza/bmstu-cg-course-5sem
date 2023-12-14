#include "serialisation/ProjectSerializer.h"

#include "application/Application.h"
#include <fstream>

void ProjectSerializer::ExportTo(const std::filesystem::path &path)
{
    std::ofstream out_file(path.string(), std::ios::binary);

    m_app.m_ui_windows->SaveState(out_file);
    m_app.m_project->GetScene()->SaveState(out_file);
}