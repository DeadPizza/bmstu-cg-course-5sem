#pragma once

#include "render/Framebuffer.h"
#include <glad/glad.h>

class GLFramebuffer : public Framebuffer
{
public:
    GLFramebuffer();
    void Bind() override;
    void SetColorAttachment(Texture &tex, uint32_t index) override;
    void SetDepthAttachment(Texture &tex) override;
    void Unbind() override;
private:
    GLuint m_descriptor;
};