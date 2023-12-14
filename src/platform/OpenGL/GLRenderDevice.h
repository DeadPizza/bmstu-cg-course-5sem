#pragma once

#include "render/RenderDevice.h"

#include <glm/glm.hpp>

class GLRenderDevice : public RenderDevice
{
public:
    GLRenderDevice();
    ~GLRenderDevice();

    void Init(GLFWwindow *window) override;
    void SetViewport(int x, int y, int w, int h) override;
    void GetViewport(int &x, int &y, int &w, int &h) override;
    void SetClearColor(const glm::vec4 &color) override;
    void Clear() override;

    void DrawVAO(VertexArray &VAO) override;
    void DrawInstanceVAO(VertexArray &VAO, int32_t count) override;

    Texture *GenerateTexture(const TextureSpecification &spec, unsigned char *data) override;
    VertexBuffer *GenerateVertexBuffer(std::initializer_list<VertexBufferAttrib> attribs) override;
    IndexBuffer *GenerateIndexBuffer(uint32_t *indices, size_t size) override;
    VertexArray *GenerateVertexArray() override;
    Shader *GenerateShader(const std::string &vertex_path, const std::string &fragment_path) override;
    UniformBuffer *GenerateUniformBuffer(size_t initial_size) override;
    Framebuffer *GenerateFramebuffer() override;
private:
    glm::ivec2 viewport_origin;
    glm::ivec2 viewport_size;
};