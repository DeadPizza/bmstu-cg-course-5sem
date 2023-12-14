#pragma once

#include <cstdint>
#include <cassert>

enum class ShaderDataType
{
    NoneType,
    Float, Float2, Float3, Float4,
    Mat3, Mat4,
    Int, Int2, Int3, Int4
};

static uint32_t SizeOfShaderDataType(const ShaderDataType &data)
{
    switch (data)
    {
        case ShaderDataType::Float: return sizeof(float);
        case ShaderDataType::Float2: return sizeof(float) * 2;
        case ShaderDataType::Float3: return sizeof(float) * 3;
        case ShaderDataType::Float4: return sizeof(float) * 4;

        case ShaderDataType::Mat3: return sizeof(float) * 3 * 3;
        case ShaderDataType::Mat4: return sizeof(float) * 4 * 4;

        case ShaderDataType::Int: return 4;
        case ShaderDataType::Int2: return 4 * 2;
        case ShaderDataType::Int3: return 4 * 3;
        case ShaderDataType::Int4: return 4 * 4;
    }
    assert(data != ShaderDataType::NoneType && "NoneType not allowed in SizeOfShaderDataType(type)");
    return 0;
}