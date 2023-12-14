#include "platform/OpenGL/GLTexture.h"

// #include <spdlog/spdlog.h>
#include <stdexcept>

static GLenum GetGLFormat(const ImageFormat format)
{
    switch (format)
    {
        case ImageFormat::R8: return GL_RED;
        case ImageFormat::RGB8:  return GL_RGB;
        case ImageFormat::RGBA8: return GL_RGBA;
        case ImageFormat::RGBA32F: return GL_RGBA;
        case ImageFormat::DEPTHBUFFER: return GL_DEPTH_COMPONENT;
    }

    return 0;
}

static GLenum GetGLInternalFormat(const ImageFormat format)
{
    switch (format)
    {
        case ImageFormat::R8: return GL_RED;
        case ImageFormat::RGB8:  return GL_RGB8;
        case ImageFormat::RGBA8: return GL_RGBA8;
        case ImageFormat::RGBA32F: return GL_RGBA32F;
        case ImageFormat::DEPTHBUFFER: return GL_DEPTH_COMPONENT;
    }

    return 0;
}

static GLenum GetGLDataType(const ImageFormat format)
{
    switch (format)
    {
        case ImageFormat::DEPTHBUFFER: return GL_FLOAT;
        case ImageFormat::RGBA32F: return GL_FLOAT;
        default: return GL_UNSIGNED_BYTE;
    }
    return 0;
}

static GLenum GetGLWrapParameter(const WrapParameter parameter)
{
    switch (parameter)
    {
        case WrapParameter::Repeat: return GL_REPEAT;
        case WrapParameter::ClampToEdge: return GL_CLAMP_TO_EDGE;
        case WrapParameter::MirroredRepeat: return GL_MIRRORED_REPEAT;
    }

    return 0;
};

static GLenum GetGLMinFilter(const MinFunction func)
{
    switch (func)
    {
        case MinFunction::Nearest: return GL_NEAREST;
        case MinFunction::Linear: return GL_LINEAR;
        case MinFunction::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
        case MinFunction::LinearMipmapNearest: return GL_LINEAR_MIPMAP_NEAREST;
        case MinFunction::NearestMipmapLinear: return GL_NEAREST_MIPMAP_LINEAR;
        case MinFunction::LinearMipmapLinear: return GL_LINEAR_MIPMAP_LINEAR;
    }
    return 0;
}

static GLenum GetGLMagFilter(const MagFunction func)
{
    switch (func)
    {
        case MagFunction::Nearest: return GL_NEAREST;
        case MagFunction::Linear: return GL_LINEAR;
    }
    return 0;
}

GLTexture::GLTexture(const TextureSpecification &spec, unsigned char *data)
    : width(spec.width),
    height(spec.height)
{
    // spdlog::info("created TEXTURE at {}", (void*)this);
    glGenTextures(1, &m_descriptor);
    glBindTexture(GL_TEXTURE_2D, m_descriptor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetGLWrapParameter(spec.s_wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetGLWrapParameter(spec.t_wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLMinFilter(spec.min));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLMagFilter(spec.mag));


    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GetGLInternalFormat(spec.img_format),
        width, height, 0,
        GetGLFormat(spec.img_format),
        GetGLDataType(spec.img_format),
        data);

    if (spec.gen_mipmaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

GLTexture::~GLTexture()
{
    glDeleteTextures(1, &m_descriptor);
}

uint32_t GLTexture::getWidth()
{
    return width;
}

uint32_t GLTexture::getHeight()
{
    return height;
}

void GLTexture::CopyFrom(Texture& other)
{
    // glCopyImageSubData()
    throw std::runtime_error("not implemented");
} 

void GLTexture::AttachAt(int n)
{
    glActiveTexture(GL_TEXTURE0 + n);
    glBindTexture(GL_TEXTURE_2D, m_descriptor);
}