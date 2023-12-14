#pragma once

#include <filesystem>

class Application;

class ProjectDeserializer
{
public:
    ProjectDeserializer(Application &app)
    : m_app(app)
    {}

    void ImportFrom(const std::filesystem::path &path);
private:
    Application &m_app;
};