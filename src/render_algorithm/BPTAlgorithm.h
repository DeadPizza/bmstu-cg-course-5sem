#pragma once

#include "render_algorithm/RenderAlgorithm.h"

#include "render/VertexArray.h"
#include "render/Shader.h"
#include "render/Framebuffer.h"

class BPTAlgorithm : public RenderAlgorithm
{
    friend class RenderAlgorithmVisitor;
public:
    bool Accept(RenderAlgorithmVisitor &) override;
    void ResetRenderState(Application &app) override;
    float GetRenderState(Application &app) override { return float(cur_frame) / float(max_frames); }
    void FramebufferSizeEvent(Application &app) override;
    void Init(Application &app) override;
    void Update(Application &app) override;
private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<Shader> m_Shader, m_cpy_Shader;
    std::unique_ptr<Framebuffer> m_Framebuffer;

    std::unique_ptr<Texture> m_Tex_1, m_Tex_2;

    int cur_frame, max_frames, perframe = 2;
    int light_path = 3, eye_path = 3;

    int m_width = 1280, m_height = 720;
};