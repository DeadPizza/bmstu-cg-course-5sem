#pragma once

#include <stddef.h>
#include <vector>
#include <string>
#include "render/ShaderDataTypes.h"

struct VertexBufferAttrib
{
    VertexBufferAttrib(const std::string &name, const ShaderDataType attr_type) :
        name(name),
        attr_type(attr_type)
    {}

    std::string name;
    ShaderDataType attr_type;
};
class VertexBuffer
{
public:
    VertexBuffer(std::initializer_list<VertexBufferAttrib> attribs)
        : attribs(attribs)
    {}

    virtual ~VertexBuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void CalculateLayout() = 0;
    virtual void SetData(void *data, size_t size) = 0;

    const std::vector<VertexBufferAttrib> &GetAttribs() { return this->attribs; }
private:
    std::vector<VertexBufferAttrib> attribs;
};