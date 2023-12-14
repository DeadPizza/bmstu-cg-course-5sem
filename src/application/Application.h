#pragma once

#include "VideoExporter.h"

#include <memory>
#include <vector>
#include <GLFW/glfw3.h>

#include "render/RenderDevice.h"
#include "ui/UIWindow.h"
#include "Project.h"
#include "render_algorithm/RenderAlgorithm.h"

class Application
{
    friend class ProjectSerializer;
    friend class ProjectDeserializer;
public:
    void Init();
    void Run();

    std::unique_ptr<RenderDevice> &GetRenderDevice() { return m_render_device; }
    std::unique_ptr<Project> &GetProject() { return m_project; }
    std::shared_ptr<RenderAlgorithm> &GetRenderAlgo() { return m_render_algo; }

    void BeginExport(int framerate) { m_videoexport.reset(new VideoExporter(framerate)); }
    void EndExport() { m_videoexport.reset(nullptr); }

    void SetRenderAlgo(std::shared_ptr<RenderAlgorithm> new_algo) { m_render_algo = new_algo; }
private:
    std::unique_ptr<Project> m_project;
    std::unique_ptr<RenderDevice> m_render_device;
    std::shared_ptr<RenderAlgorithm> m_render_algo;
    std::unique_ptr<UIWindow> m_ui_windows;

    std::unique_ptr<VideoExporter> m_videoexport;
    GLFWwindow *m_window;
};