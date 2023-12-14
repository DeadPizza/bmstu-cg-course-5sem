#include "application/Application.h"
#include "render_algorithm/RayTracingAlgorithm.h"

#include "render_algorithm/RenderAlgorithmVisitor.h"
#include <glm/glm.hpp>


void RayTracingAlgorithm::Init(Application &app)
{
    auto &render_device = app.GetRenderDevice();

    m_VAO.reset(render_device->GenerateVertexArray());

    auto *VBO = render_device->GenerateVertexBuffer({
            {"aPosition", ShaderDataType::Float2},
        });

    glm::vec2 vertices[4] = {
        { 1,  1},
        { 1, -1},
        {-1, -1},
        {-1,  1}
    };

    unsigned int indices[6] = {
        0, 1, 3,
        1, 2, 3
    };

    auto *EBO = render_device->GenerateIndexBuffer(indices, 6);


    m_VAO->Bind();
    VBO->Bind();
    EBO->Bind();

    m_VAO->SetVertexBuffer(*VBO);
    m_VAO->SetIndexBuffer(*EBO);

    VBO->SetData(vertices, sizeof(vertices));
    VBO->CalculateLayout();

    m_VAO->Unbind();
    VBO->Unbind();
    EBO->Unbind();

    TextureSpecification spec;
    spec.width = 1280;
    spec.height = 720;

    m_Tex_1.reset(render_device->GenerateTexture(spec, nullptr));
    m_Tex_2.reset(render_device->GenerateTexture(spec, nullptr));

    m_Framebuffer.reset(render_device->GenerateFramebuffer());

    m_Shader.reset(render_device->GenerateShader("../shaders/ray_tracing.vs", "../shaders/ray_tracing.fs"));
}

void RayTracingAlgorithm::Update(Application &app)
{
    auto &render_device = app.GetRenderDevice();

    m_Shader->Bind();

    int x, y, w, h;
    render_device->GetViewport(x, y, w, h);
    m_Shader->SetFloat2("u_resolution", { w, h });
    m_Shader->SetFloat("u_time", glfwGetTime());
    m_Shader->SetInt("u_samples_perframe", 1);
    m_Shader->SetInt("u_samples", 1);

    m_Shader->SetFloat3("u_campos", app.GetProject()->GetCamera()->GetPosition());
    m_Shader->SetMat4("u_raydirmod", app.GetProject()->GetCamera()->GetViewMatrix());
    // m_Shader->SetFloat3("u_campos", glm::vec3{ 2.78, 2.73, -8 });
    // m_Shader->SetMat4("u_raydirmod", glm::mat4(1));

    app.GetProject()->GetScene()->GetRawUBO()->Bind(0);
    m_Shader->BindUniformBlock("MapData", 0);

    m_VAO->Bind();
    render_device->DrawVAO(*m_VAO);
}

bool RayTracingAlgorithm::Accept(RenderAlgorithmVisitor &visitor)
{
    return visitor.Visit(*this);
}