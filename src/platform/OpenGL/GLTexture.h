#pragma once

#include <stdint.h>

#include "render/Texture.h"

#include "glad/glad.h"


class GLTexture : public Texture
{
public:
    GLTexture(const TextureSpecification &spec, unsigned char *data);
    ~GLTexture();

    uint32_t getWidth() override;
    uint32_t getHeight() override;

    void CopyFrom(Texture& other) override;
    void AttachAt(int n) override;

    GLuint GetDescriptor() { return m_descriptor; }
private:
    GLuint m_descriptor;
    int width, height;
};
