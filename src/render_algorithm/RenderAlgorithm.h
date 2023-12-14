#pragma once

#include <memory>

#include "render/RenderDevice.h"

class Application;
class RenderAlgorithmVisitor;

class RenderAlgorithm
{
public:
    virtual ~RenderAlgorithm() = default;

    virtual bool Accept(RenderAlgorithmVisitor &) = 0;
    virtual void ResetRenderState(Application &app) = 0;
    virtual float GetRenderState(Application &app) = 0;

    virtual void FramebufferSizeEvent(Application &app) {}

    virtual void Init(Application &app) = 0;
    virtual void Update(Application &app) = 0;
};