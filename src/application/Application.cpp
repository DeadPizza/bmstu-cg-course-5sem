#include <glad/glad.h>
#include "application/Application.h"
#include "Application.h"

#include "platform/OpenGL/GLRenderDevice.h"

#include <GLFW/glfw3.h>
#include <stdexcept>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "managers/InputManager.h"

#include "ui/UIComposite.h"
#include "ui/UISceneEditor.h"
#include "ui/UIAlgorithmProperties.h"
#include "ui/UIAnimator.h"

#include <iostream>
#include "camera/CameraController.h"
#include <cstdio>

#include "serialisation/ProjectSerializer.h"
#include "serialisation/ProjectDeserializer.h"

#include "ImGuiFileDialog.h"

void Application::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_RESIZABLE, false);
    // glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(1280, 720, "LightWave Sim", nullptr, nullptr);

    if (!m_window)
    {
        throw std::runtime_error("failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);

    static auto &render_algo_ptr = m_render_algo;
    static auto *this_ptr = this;

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *win, int w, int h)
        {
            // std::printf("invoke\n");

            if (render_algo_ptr.get())
            {
                render_algo_ptr->FramebufferSizeEvent(*this_ptr);
            }
        });

    InputManager::Init(m_window);

    ImGui::CreateContext();
    auto &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../fonts/clear-sans.ttf", 20, NULL, io.Fonts->GetGlyphRangesCyrillic());
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_render_device = std::make_unique<GLRenderDevice>();
    m_render_device->Init(m_window);

    m_project = std::make_unique<Project>(*this);

    UIVectorOfPtrs tmp_vec;
    tmp_vec.emplace_back(std::make_unique<UISceneEditor>());
    tmp_vec.emplace_back(std::make_unique<UIAlgorithmProperties>());
    tmp_vec.emplace_back(std::make_unique<UIAnimator>());

    m_ui_windows = std::make_unique<UIComposite>(std::move(tmp_vec));

    m_ui_windows->Init(*this);

    m_render_device->SetViewport(0, 0, 1280, 720);
}

void ShowMainMenu()
{
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Файл"))
    {
        // if (ImGui::MenuItem("Новый проект")) {}
        // ImGui::Separator();
        if (ImGui::MenuItem("Открыть проект"))
        {
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".bin", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);
        }
        // if (ImGui::MenuItem("Сохранить проект")) {}
        if (ImGui::MenuItem("Сохранить проект как"))
        {
            ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlgKey", "Choose File", ".bin", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);
        }
        // ImGui::Separator();
        // if (ImGui::MenuItem("Закрыть проект")) {}
        // ImGui::Separator();
        // if (ImGui::MenuItem("Выйти")) {}
        ImGui::EndMenu();
    }
    // if (ImGui::BeginMenu("Действия"))
    // {
    //     if (ImGui::MenuItem("Отменить")) {}
    //     if (ImGui::MenuItem("Повторить")) {}
    //     ImGui::EndMenu();
    // }
    // if (ImGui::BeginMenu("Помощь"))
    // {
    //     if (ImGui::MenuItem("Управление")) {}
    //     ImGui::Separator();
    //     if (ImGui::MenuItem("Об авторе")) {}
    //     if (ImGui::MenuItem("О приложении")) {}

    //     ImGui::EndMenu();
    // }
    ImGui::EndMainMenuBar();
};

void Application::Run()
{
    CameraController controls;
    controls.SetCamera(m_project->GetCamera());
    bool state = false;

    // ProjectDeserializer des(*this);
    // if (std::filesystem::exists("test_project.bin"))
    // {
    //     des.ImportFrom("test_project.bin");
    // }

    // FILE *video_out = popen("ffmpeg -y -f rawvideo -s 1280x720 -pix_fmt rgb24 -r 25 -i - -vf vflip -an -b:v 1000k test.mp4", "w");
    // char *frame_buf = new char[1280 * 720 * 3];

    // std::cout << (void*)video_out;
    while (!glfwWindowShouldClose(m_window))
    {
        m_render_device->Clear();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ShowMainMenu();
        // ImGui::ShowDemoWindow();

        if (InputManager::GetKeyDown(GLFW_KEY_X))
        {
            state = !state;
        }

        bool reset_render = false;
        if (state)
        {
            reset_render |= controls.Update();
        }
        else
        {
            if (m_ui_windows->Update(*this))
            {
                reset_render = true;
                m_project->GetScene()->SyncToRenderer(*this);
            }
        }
        if (reset_render)
            m_render_algo->ResetRenderState(*this);


        m_render_algo->Update(*this);
        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
        {
            // action if OK
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                // action
                ProjectDeserializer des(*this);
                des.ImportFrom(filePathName);
                m_render_algo->ResetRenderState(*this);
            }

            // close
            ImGuiFileDialog::Instance()->Close();
        }

        if (ImGuiFileDialog::Instance()->Display("SaveFileDlgKey"))
        {
            // action if OK
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                // action
                ProjectSerializer ser(*this);
                ser.ExportTo(filePathName);
            }

            // close
            ImGuiFileDialog::Instance()->Close();
        }

        if (m_videoexport.get() && m_render_algo->GetRenderState(*this) == 1)
        {
            m_videoexport->CaptureFrame();
        }
        // glReadPixels(0, 0, 1280, 720, GL_RGB, GL_UNSIGNED_BYTE, frame_buf);
        // fwrite(frame_buf, 1280 * 720 * 3, 1, video_out);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        InputManager::OnFrameEnd();
        glfwPollEvents();
        glfwSwapBuffers(m_window);
    }
    // ProjectSerializer ser(*this);
    // ser.ExportTo("test_project.bin");
    // fclose(video_out);
}