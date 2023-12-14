#pragma once

#include "render/IndexBuffer.h"
#include <stddef.h>

#include <glad/glad.h>

class GLIndexBuffer : public IndexBuffer
{
public:
    GLIndexBuffer(GLuint *indices, size_t size);
    ~GLIndexBuffer();

    void Bind() override;
    void Unbind() override;

    int GetCount() override;
private:
    GLuint m_descriptor;
    size_t m_count;
};