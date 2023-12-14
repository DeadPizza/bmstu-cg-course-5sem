#pragma once

#include "render/VertexBuffer.h"
#include "render/IndexBuffer.h"

class VertexArray
{
public:
    virtual ~VertexArray() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void SetVertexBuffer(VertexBuffer &vbo) = 0;
    virtual VertexBuffer &GetVertexBuffer() = 0;
    virtual void SetIndexBuffer(IndexBuffer &ebo) = 0;
    virtual IndexBuffer &GetIndexBuffer() = 0;
};