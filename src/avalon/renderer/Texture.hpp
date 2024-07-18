#pragma once

#include "avalon/core/Core.hpp"
#include <glad/glad.h>

#include "stb_image.h"

class Texture {
private:
    GLuint textureID;
    int width, height, channel;

public:
    explicit Texture(const std::string& filePath) {
        generateAndLoad(filePath.c_str());
    }

private:
    void generateAndLoad(const char *filePath) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set the texture parameters
        // Repeat the image in both directions
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // When stretching the image, pixelate
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // When shrinking an image, pixelate
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        auto data = stbi_load(filePath, &width, &height, &channel, 0);

        if (data && (channel == 3 || channel == 4)) {
            glTexImage2D(GL_TEXTURE_2D, 0, (channel == 3 ? GL_RGB : GL_RGBA), width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         data);

        } else {
            std::cerr << "Error loading texture: " << filePath;
        }

        stbi_image_free(data);
    }

public:
    void bind() const {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

