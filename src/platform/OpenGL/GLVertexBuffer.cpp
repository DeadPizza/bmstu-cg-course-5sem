#include "platform/OpenGL/GLVertexBuffer.h"
#include <stdexcept>

GLVertexBuffer::GLVertexBuffer(std::initializer_list<VertexBufferAttrib> attribs)
    : VertexBuffer(attribs)
{
    glGenBuffers(1, &m_descriptor);
}

GLVertexBuffer::~GLVertexBuffer()
{
    glDeleteBuffers(1, &m_descriptor);
}

void GLVertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_descriptor);
}

void GLVertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLVertexBuffer::CalculateLayout()
{
    uint32_t stride = 0;
    for (const auto &cur_attrib : this->GetAttribs())
    {
        stride += SizeOfShaderDataType(cur_attrib.attr_type);
    }
    uint32_t cur_offset = 0;
    for (int i = 0; i < this->GetAttribs().size(); ++i)
    {
        // TODO: add non float attrs
        uint32_t cur_attr_size = SizeOfShaderDataType(this->GetAttribs()[i].attr_type);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, cur_attr_size / sizeof(float), GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(cur_offset));
        cur_offset += cur_attr_size;
    }
    // SizeOfShaderDataType()
}

void GLVertexBuffer::SetData(void *data, size_t size)
{
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    // throw std::runtime_error("Not implemented!");
}
