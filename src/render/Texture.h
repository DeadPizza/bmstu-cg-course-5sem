#pragma once

#include <stdint.h>

enum class ImageFormat
{
    None = 0,
    R8,
    RGB8,
    RGBA8,
    RGBA32F,
    DEPTHBUFFER,
    STENCILBUFFER
};

enum class WrapParameter
{
    Repeat,
    MirroredRepeat,
    ClampToEdge
};

enum class MinFunction
{
    Nearest,
    Linear,
    NearestMipmapNearest,
    LinearMipmapNearest,
    NearestMipmapLinear,
    LinearMipmapLinear
};

enum class MagFunction
{
    Nearest,
    Linear
};

struct TextureSpecification
{
    uint32_t width = 1;
    uint32_t height = 1;
    ImageFormat img_format = ImageFormat::RGBA8;
    bool gen_mipmaps = true;
    WrapParameter s_wrap = WrapParameter::Repeat;
    WrapParameter t_wrap = WrapParameter::Repeat;
    MinFunction min = MinFunction::NearestMipmapLinear;
    MagFunction mag = MagFunction::Linear;
};

class Texture
{
public:
    virtual ~Texture() = default;

    virtual uint32_t getWidth() = 0;
    virtual uint32_t getHeight() = 0;

    virtual void AttachAt(int n) = 0;
    virtual void CopyFrom(Texture& other) = 0;
};