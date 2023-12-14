#include "platform/OpenGL/GLVertexArray.h"

// #include <spdlog/spdlog.h>

GLVertexArray::GLVertexArray()
{
    // spdlog::info("created MODEL at {}", (void*)this);
    glGenVertexArrays(1, &m_descriptor);
}

GLVertexArray::~GLVertexArray()
{
    glDeleteVertexArrays(1, &m_descriptor);
}

void GLVertexArray::Bind()
{
    glBindVertexArray(m_descriptor);
}

void GLVertexArray::Unbind()
{
    glBindVertexArray(0);
}

void GLVertexArray::SetVertexBuffer(VertexBuffer &vbo)
{
    m_vbo = reinterpret_cast<GLVertexBuffer *>(&vbo);
}

VertexBuffer &GLVertexArray::GetVertexBuffer()
{
    return *m_vbo;
}

void GLVertexArray::SetIndexBuffer(IndexBuffer &ebo)
{
    m_ebo = reinterpret_cast<GLIndexBuffer *>(&ebo);
}

IndexBuffer &GLVertexArray::GetIndexBuffer()
{
    return *m_ebo;
}
