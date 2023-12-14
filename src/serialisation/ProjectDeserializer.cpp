#include "serialisation/ProjectDeserializer.h"
#include "application/Application.h"
#include <fstream>
#include <iostream>

void ProjectDeserializer::ImportFrom(const std::filesystem::path &path)
{
    std::cout << "importing from: " << path.string() << "\n";
    std::ifstream in_file(path.string(), std::ios::binary);

    m_app.m_ui_windows->RestoreState(in_file);
    m_app.m_project->GetScene()->RestoreState(in_file);
}