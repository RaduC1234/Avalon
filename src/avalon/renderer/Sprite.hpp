#pragma once

#include <utility>

#include "avalon/core/Core.hpp"

#include "Texture.hpp"
#include "glm/vec2.hpp"

class Sprite {
    Ref<Texture> texture;
    std::array<glm::vec2, 4> texCoords;

public:

    Sprite(const Ref<Texture> &texture) : texture(texture) {
        texCoords = {
                glm::vec2(0, 0),
                glm::vec2(0, 1),
                glm::vec2(1, 1),
                glm::vec2(1, 0)
        };


    Sprite(const Ref<Texture> &texture, const std::array<glm::vec2, 4> &texCoords) : texture(texture),
                                                                                     texCoords(texCoords) {}

    const Ref<Texture> &getTexture() const {
        return texture;
    }

    const std::array<glm::vec2, 4> &getTexCoords() const {
        return texCoords;
    }
};

class SpriteSheet {
    Ref<Texture> texture;
    std::vector<Sprite> sprites;

    SpriteSheet(Ref<Texture> texture, int spriteWidth, int spriteHeight, int numSprites, int spacing) : texture(std::move(texture)) {

        int currentX = 0;
        int currentY = texture->getHeight() - spriteHeight;
    }
};

class SpriteSheetLoader {

};
