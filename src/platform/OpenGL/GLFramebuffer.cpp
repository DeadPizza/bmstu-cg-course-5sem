#include "platform/OpenGL/GLFramebuffer.h"
#include "platform/OpenGL/GLTexture.h"

GLFramebuffer::GLFramebuffer()
{
    glGenFramebuffers(1, &m_descriptor);
}

void GLFramebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_descriptor);
}

void GLFramebuffer::SetColorAttachment(Texture &tex, uint32_t index)
{
    GLTexture &real_tex = reinterpret_cast<GLTexture &>(tex);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, real_tex.GetDescriptor(), 0);
}

void GLFramebuffer::SetDepthAttachment(Texture &tex)
{
    GLTexture &real_tex = reinterpret_cast<GLTexture &>(tex);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, real_tex.GetDescriptor(), 0);
}

void GLFramebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
