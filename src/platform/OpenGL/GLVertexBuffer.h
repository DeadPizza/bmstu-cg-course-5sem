#pragma once

#include "render/VertexBuffer.h"

#include <stddef.h>
#include <glad/glad.h>

    class GLVertexBuffer : public VertexBuffer
    {
    public:
        GLVertexBuffer(std::initializer_list<VertexBufferAttrib> attribs);
        ~GLVertexBuffer();

        void Bind() override;
        void Unbind() override;

        void CalculateLayout() override;
        void SetData(void *data, size_t size) override;
    private:
        GLuint m_descriptor;
    };