#pragma once

#include <filesystem>

class Application;

class ProjectSerializer
{
public:
    ProjectSerializer(Application &app)
    : m_app(app)
    {}

    void ExportTo(const std::filesystem::path &path);
private:
    Application &m_app;
};