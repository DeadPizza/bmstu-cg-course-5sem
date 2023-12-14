#include "application/Application.h"
#include "render_algorithm/BPTAlgorithm.h"

#include "render_algorithm/RenderAlgorithmVisitor.h"

void BPTAlgorithm::Init(Application &app)
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
    spec.img_format = ImageFormat::RGBA32F;

    m_Tex_1.reset(render_device->GenerateTexture(spec, nullptr));
    m_Tex_2.reset(render_device->GenerateTexture(spec, nullptr));

    m_Framebuffer.reset(render_device->GenerateFramebuffer());

    m_Shader.reset(render_device->GenerateShader("../shaders/bidir_pathtrace.vs", "../shaders/bidir_pathtrace.fs"));
    m_cpy_Shader.reset(render_device->GenerateShader("../shaders/bidir_pathtrace.vs", "../shaders/frag_draw_tex.fs"));

    cur_frame = 0;
    max_frames = 128;
}

void BPTAlgorithm::Update(Application &app)
{
    if (cur_frame < max_frames)
    {
        ++cur_frame;
        // ResetState();
        m_Framebuffer->Bind();
        m_Framebuffer->SetColorAttachment(*m_Tex_2, 0);
        app.GetRenderDevice()->Clear();
        // glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferObject);
        // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fOutputTexture, 0);
        // glClearColor(0.6, 0, 0, 1);
        // glClear(GL_COLOR_BUFFER_BIT);
        m_VAO->Bind();
        m_Shader->Bind();
        // glBindVertexArray(m_fullscreen_model);
        // m_shader.use();

        m_Shader->SetFloat2("u_resolution", { m_width, m_height });
        m_Shader->SetFloat("u_time", glfwGetTime());
        m_Shader->SetInt("u_samples_perframe", perframe);
        m_Shader->SetInt("u_samples", max_frames);
        m_Shader->SetInt("u_lightpathlen", light_path);
        m_Shader->SetInt("u_eyepathlen", eye_path);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, fInputTexture);
        m_Tex_1->AttachAt(0);
        m_Shader->SetInt("u_texture", 0);

        // auto &cam_pos = camera_controls.GetVector();
        m_Shader->SetFloat3("u_campos", app.GetProject()->GetCamera()->GetPosition());
        m_Shader->SetMat4("u_raydirmod", app.GetProject()->GetCamera()->GetViewMatrix());

        // glUniformMatrix4fv(glGetUniformLocation(m_shader.ID, "u_raydirmod"), 1, GL_FALSE, glm::value_ptr(camera_controls.GetTransMatrix()));

        app.GetProject()->GetScene()->GetRawUBO()->Bind(0);
        m_Shader->BindUniformBlock("MapData", 0);
        // glBindBufferBase(GL_UNIFORM_BUFFER, 0, scene.getUBO());
        // unsigned int map_data = glGetUniformBlockIndex(m_shader.ID, "MapData");
        // glUniformBlockBinding(m_shader.ID, map_data, 0);

        app.GetRenderDevice()->DrawVAO(*m_VAO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        m_Framebuffer->SetColorAttachment(*m_Tex_1, 0);
        // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fInputTexture, 0);

        // m_copyshader.use();
        m_cpy_Shader->Bind();
        m_Tex_2->AttachAt(0);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, fOutputTexture);
        m_cpy_Shader->SetInt("u_texture", 0);
        m_cpy_Shader->SetFloat("u_coef", 1);
        m_cpy_Shader->SetBool("do_postproc", false);
        m_cpy_Shader->SetFloat2("u_resolution", { m_width, m_height });
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        app.GetRenderDevice()->DrawVAO(*m_VAO);

        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        m_Framebuffer->Unbind();
    }
    m_cpy_Shader->Bind();
    m_Tex_2->AttachAt(0);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, fOutputTexture);
    m_cpy_Shader->SetInt("u_texture", 0);
    m_cpy_Shader->SetBool("do_postproc", true);
    m_cpy_Shader->SetFloat("u_coef", float(max_frames) / float(cur_frame));
    // std::printf("cur ratio: %f\n", float(max_frames) / float(cur_frame));
    m_cpy_Shader->SetFloat2("u_resolution", { m_width, m_height });
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    app.GetRenderDevice()->DrawVAO(*m_VAO);
    // m_copyshader.use();
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, fOutputTexture);
    // m_copyshader.setInt("u_texture", 0);
    // m_copyshader.setFloat("u_coef", float(uniforms.u_samples_total) / float(cur_frame));
    // m_copyshader.setVec2("u_resolution", uniforms.u_resolution.x, uniforms.u_resolution.y);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void BPTAlgorithm::FramebufferSizeEvent(Application &app)
{
    int x, y, w, h;
    app.GetRenderDevice()->GetViewport(x, y, w, h);
    m_width = w;
    m_height = h;

    TextureSpecification spec;
    spec.width = w;
    spec.height = h;
    spec.img_format = ImageFormat::RGBA32F;

    m_Tex_1.reset(app.GetRenderDevice()->GenerateTexture(spec, nullptr));
    m_Tex_2.reset(app.GetRenderDevice()->GenerateTexture(spec, nullptr));
    this->ResetRenderState(app);
}

bool BPTAlgorithm::Accept(RenderAlgorithmVisitor &visitor)
{
    return visitor.Visit(*this);
}

void BPTAlgorithm::ResetRenderState(Application &app)
{
    cur_frame = 0;
    m_Framebuffer->Bind();
    m_Framebuffer->SetColorAttachment(*m_Tex_1, 0);
    app.GetRenderDevice()->Clear();
    m_Framebuffer->Unbind();
    // cur_frame = 0;
    // glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferObject);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fInputTexture, 0);
    // glClearColor(0, 0, 0, 0);
    // glClear(GL_COLOR_BUFFER_BIT);
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
}