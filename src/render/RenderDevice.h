#pragma once

#include <GLFW/glfw3.h>

#include "render/Texture.h"
#include "render/VertexBuffer.h"
#include "render/IndexBuffer.h"
#include "render/VertexArray.h"
#include "render/Shader.h"
#include "render/Framebuffer.h"
#include "render/UniformBuffer.h"

#include <glm/glm.hpp>


class RenderDevice
{
public:
    virtual ~RenderDevice() = default;

    virtual void Init(GLFWwindow *window) = 0;
    virtual void SetViewport(int x, int y, int w, int h) = 0;
    virtual void GetViewport(int &x, int &y, int &w, int &h) = 0;
    virtual void SetClearColor(const glm::vec4 &color) = 0;
    virtual void Clear() = 0;

    virtual void DrawVAO(VertexArray &VAO) = 0;
    virtual void DrawInstanceVAO(VertexArray &VAO, int32_t count) = 0;

    virtual Texture *GenerateTexture(const TextureSpecification &spec, unsigned char *data) = 0;
    virtual VertexBuffer *GenerateVertexBuffer(std::initializer_list<VertexBufferAttrib> attribs) = 0;
    virtual IndexBuffer *GenerateIndexBuffer(uint32_t *indices, size_t size) = 0;
    virtual VertexArray *GenerateVertexArray() = 0;
    virtual Shader *GenerateShader(const std::string &vertex_path, const std::string &fragment_path) = 0;
    virtual UniformBuffer *GenerateUniformBuffer(size_t initial_size) = 0;
    virtual Framebuffer *GenerateFramebuffer() = 0;
};