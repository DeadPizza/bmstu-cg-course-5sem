#pragma once

#include "render/Texture.h"

class Framebuffer
{
public:
    virtual ~Framebuffer() = default;

    virtual void Bind() = 0;
    virtual void SetColorAttachment(Texture &tex, uint32_t index) = 0;
    virtual void SetDepthAttachment(Texture &tex) = 0;
    virtual void Unbind() = 0;
};