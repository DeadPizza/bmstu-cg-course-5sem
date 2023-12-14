#include "platform/OpenGL/GLIndexBuffer.h"

GLIndexBuffer::GLIndexBuffer(GLuint *indices, size_t size):
    m_count(size)
{
    glGenBuffers(1, &m_descriptor);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * size, indices, GL_STATIC_DRAW);
}

GLIndexBuffer::~GLIndexBuffer()
{
    glDeleteBuffers(1, &m_descriptor);
}

void GLIndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_descriptor);
}

void GLIndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int GLIndexBuffer::GetCount()
{
    return m_count;   
}
