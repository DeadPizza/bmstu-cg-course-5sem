#pragma once

#include "render_algorithm/RenderAlgorithm.h"

#include "render/VertexArray.h"
#include "render/Shader.h"
#include "render/Framebuffer.h"

class RayTracingAlgorithm : public RenderAlgorithm
{
public:
    RayTracingAlgorithm() {}

    bool Accept(RenderAlgorithmVisitor &) override;
    void ResetRenderState(Application &app) override {}
    float GetRenderState(Application &app) override {return 1;}

    void Init(Application &app) override;
    void Update(Application &app) override;

private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Framebuffer> m_Framebuffer;

    std::unique_ptr<Texture> m_Tex_1, m_Tex_2;
};