#pragma once

#include "render/VertexArray.h"

#include "platform/OpenGL/GLVertexBuffer.h"
#include "platform/OpenGL/GLIndexBuffer.h"

#include <glad/glad.h>


class GLVertexArray : public VertexArray
{
public:
    GLVertexArray();
    ~GLVertexArray();

    void Bind() override;
    void Unbind() override;

    void SetVertexBuffer(VertexBuffer &vbo) override;
    VertexBuffer &GetVertexBuffer() override;
    void SetIndexBuffer(IndexBuffer &ebo) override;
    IndexBuffer &GetIndexBuffer() override;
private:
    GLuint m_descriptor;

    GLVertexBuffer *m_vbo;
    GLIndexBuffer *m_ebo;
};