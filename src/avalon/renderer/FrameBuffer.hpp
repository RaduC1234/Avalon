#pragma once

#include "avalon/core/Core.hpp"

#include <glad/glad.h>

class FrameBuffer {
public:
    FrameBuffer() = default;

    FrameBuffer(int width, int height, bool useDepth = true) : width(width), height(height), useDepth(useDepth) {
        create();
    }

    ~FrameBuffer() {
        remove();
    }


    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, width, height);
    }

    void unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void resize(int newWidth, int newHeight) {
        remove();

        width = newWidth;
        height = newHeight;

        create();
    }

    uint32_t getTextureId() const {
        return this->texture;
    }

private:

    void create() {
        // Create the framebuffer
        glGenBuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Generate a texture ID for the framebuffer texture.
        glGenTextures(1, &texture);

        // Bind the texture as a 2D texture.
        glBindTexture(GL_TEXTURE_2D, texture);

        // Specify the texture's width, height, format, and type.
        // We're allocating memory for the texture but not initializing it with any data (hence `NULL`).
        // The texture format is set to GL_RGBA (for both the internal format and the data format), meaning
        // it will store Red, Green, Blue, and Alpha channels, each with 8 bits.
        // `GL_UNSIGNED_BYTE` specifies that each color component will be stored as an unsigned byte.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        // Set the texture's minification filter to linear filtering.
        // This means that when the texture is scaled down (minified), OpenGL will interpolate the pixel values
        // in a smooth manner, making the texture appear less pixelated.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Set the texture's magnification filter to linear filtering.
        // This means that when the texture is scaled up (magnified), OpenGL will interpolate the pixel values
        // in a smooth manner, preventing the texture from looking blocky or pixelated.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Attach the texture to the framebuffer.
        // `GL_FRAMEBUFFER` specifies that we are working with the currently bound framebuffer.
        // `GL_COLOR_ATTACHMENT0` indicates that this texture will be used as the color attachment (the target buffer
        // for color rendering) for the framebuffer.
        // `GL_TEXTURE_2D` indicates that the attachment is a 2D texture, and `texture` is the ID of that texture.
        // The last argument (`0`) specifies the mipmap level of the texture. For most purposes, this should be 0,
        // which is the base level of the texture (no mipmapping).
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

        if (useDepth) {
            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        }

        // Check if the framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            AV_CORE_ERROR("Error: Framebuffer is not complete!");
        }

        // Unbind the framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void remove() {
        if (fbo) glDeleteFramebuffers(1, &fbo);
        if (texture) glDeleteTextures(1, &texture);
        if (rbo && useDepth) glDeleteRenderbuffers(1, &rbo);

        fbo = 0;
        texture = 0;
        rbo = 0;
    }

    int width{}, height{};
    uint32_t fbo{}, texture{}, rbo{};
    bool useDepth{};
};
