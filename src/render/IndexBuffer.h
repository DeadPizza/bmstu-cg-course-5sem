#pragma once

#include <stddef.h>

class IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual int GetCount() = 0;
};
