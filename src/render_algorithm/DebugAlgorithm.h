#pragma once

#include "render_algorithm/RenderAlgorithm.h"

#include "render/VertexArray.h"
#include "render/Shader.h"

class DebugAlgorithm : public RenderAlgorithm
{
public:
    DebugAlgorithm() {}

    bool Accept(RenderAlgorithmVisitor &) override;
    void ResetRenderState(Application &app) override {}
    float GetRenderState(Application &app) override { return 1; }

    void Init(Application &app) override;
    void Update(Application &app) override;

private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<Shader> m_shader;
};