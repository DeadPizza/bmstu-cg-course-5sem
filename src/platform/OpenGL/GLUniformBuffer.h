#pragma once

#include "render/UniformBuffer.h"
#include <glad/glad.h>

    class GLUniformBuffer: public UniformBuffer
    {
    public:
        GLUniformBuffer(size_t initial_size);
        ~GLUniformBuffer();

        void Bind(int32_t index) override;
        void SubData(void *data, size_t offset, size_t size) override;
        void Unbind() override;
    private:
        GLuint m_descriptor;
    };