#pragma once

#include <cstdint>
#include <cstddef>


class UniformBuffer
{
public:
    virtual ~UniformBuffer() = default;
    virtual void Bind(int32_t index) = 0;
    virtual void SubData(void *data, size_t offset, size_t size) = 0;
    virtual void Unbind() = 0;
};
