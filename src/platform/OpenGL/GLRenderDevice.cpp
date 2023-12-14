#include <glad/glad.h>

#include "platform/OpenGL/GLRenderDevice.h"
#include "platform/OpenGL/GLTexture.h"
#include "platform/OpenGL/GLFramebuffer.h"
#include "platform/OpenGL/GLUniformBuffer.h"
#include "platform/OpenGL/GLVertexArray.h"
#include "platform/OpenGL/GLVertexBuffer.h"
#include "platform/OpenGL/GLIndexBuffer.h"
#include "platform/OpenGL/GLShader.h"

#include <stdexcept>

GLRenderDevice::GLRenderDevice()
{

}

GLRenderDevice::~GLRenderDevice()
{

}

void GLRenderDevice::Init(GLFWwindow *window)
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("unable to load GLAD!");
    }

    static RenderDevice* render_device_ptr = this;

    static GLFWframebuffersizefun prev_FramebufferSizeCallback = glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
        {
            render_device_ptr->SetViewport(0, 0, width, height);
            if (prev_FramebufferSizeCallback)
                prev_FramebufferSizeCallback(window, width, height);
        });
}

void GLRenderDevice::SetViewport(int x, int y, int w, int h)
{
    glViewport(x, y, w, h);

    viewport_origin.x = x;
    viewport_origin.y = y;
    viewport_size.x = w;
    viewport_size.y = h;
}

void GLRenderDevice::GetViewport(int &x, int &y, int &w, int &h)
{
    x = viewport_origin.x;
    y = viewport_origin.y;
    w = viewport_size.x;
    h = viewport_size.y;
}

void GLRenderDevice::SetClearColor(const glm::vec4 &color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void GLRenderDevice::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderDevice::DrawVAO(VertexArray &VAO)
{
    VAO.Bind();
    glDrawElements(GL_TRIANGLES, VAO.GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GLRenderDevice::DrawInstanceVAO(VertexArray &VAO, int32_t count)
{
    VAO.Bind();
    // glDrawArraysInstanced(GL_TRIANGLES, 0, VAO.GetIndexBuffer().GetCount(), count);
    glDrawElementsInstanced(GL_TRIANGLES, VAO.GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, 0, count);
}

Texture *GLRenderDevice::GenerateTexture(const TextureSpecification &spec, unsigned char *data)
{
    return new GLTexture(spec, data);
}

VertexBuffer *GLRenderDevice::GenerateVertexBuffer(std::initializer_list<VertexBufferAttrib> attribs)
{
    return new GLVertexBuffer(attribs);
}

IndexBuffer *GLRenderDevice::GenerateIndexBuffer(uint32_t *indices, size_t size)
{
    return new GLIndexBuffer(indices, size);
}

VertexArray *GLRenderDevice::GenerateVertexArray()
{
    return new GLVertexArray();
}

Shader *GLRenderDevice::GenerateShader(const std::string &vertex_path, const std::string &fragment_path)
{
    return new GLShader(vertex_path, fragment_path);
}

UniformBuffer *GLRenderDevice::GenerateUniformBuffer(size_t initial_size)
{
    return new GLUniformBuffer(initial_size);
}

Framebuffer *GLRenderDevice::GenerateFramebuffer()
{
    return new GLFramebuffer();
}