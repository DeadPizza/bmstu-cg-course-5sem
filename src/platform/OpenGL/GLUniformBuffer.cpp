#include "platform/OpenGL/GLUniformBuffer.h"

GLUniformBuffer::GLUniformBuffer(size_t initial_size)
{
    glGenBuffers(1, &m_descriptor);
    glBindBuffer(GL_UNIFORM_BUFFER, m_descriptor);
    glBufferData(GL_UNIFORM_BUFFER, initial_size, NULL, GL_DYNAMIC_DRAW);
}

GLUniformBuffer::~GLUniformBuffer()
{
    glDeleteBuffers(1, &m_descriptor);
}

void GLUniformBuffer::Bind(int32_t index)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_descriptor);
    glBindBufferBase(GL_UNIFORM_BUFFER, index, m_descriptor);
}

void GLUniformBuffer::SubData(void *data, size_t offset, size_t size)
{
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void GLUniformBuffer::Unbind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
