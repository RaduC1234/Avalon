#pragma once

#include <utility>

#include "avalon/core/Core.hpp"

#include "Texture.hpp"
#include "glm/vec2.hpp"

using TextureCoords = std::array<glm::vec2, 4>;

class Sprite {
public:

/**
     * Index in the spritesheet.
     */
    int index;

    Ref<Texture> texture;

    std::array<glm::vec2, 4> texCoords = { // image flipped on x to be displayed correctly
            glm::vec2(1, 0),
            glm::vec2(1, 1),
            glm::vec2(0, 1),
            glm::vec2(0, 0)
    };

    // this is for when there is no texture applied to the object
    Sprite() = default;

    Sprite(Ref<Texture> texture) : index(-1), texture(std::move(texture)) {}

    Sprite(Ref<Texture> texture, std::array<glm::vec2, 4> texCoors, int index) : texture(std::move(texture)), texCoords(texCoors), index(index) {}

    bool operator==(const Sprite& other) const {
        return index == other.index && texture == other.texture;
    }

};

struct SpriteSheet {
    Ref<Texture> texture;
    std::vector<Sprite> sprites;
};
