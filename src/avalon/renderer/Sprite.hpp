#pragma once

#include <utility>

#include "avalon/core/Core.hpp"

#include "Texture.hpp"
#include "glm/vec2.hpp"

struct Sprite {

    /**
     * Index in the spritesheet.
     */
    int index;

    Ref<Texture> texture;

    std::array<glm::vec2, 4> texCoords = {
            glm::vec2(0, 0),
            glm::vec2(0, 1),
            glm::vec2(1, 1),
            glm::vec2(1, 0)
    };

    // this is for when there is no texture applied to the object
    Sprite() = default;

    Sprite(const Ref<Texture> &texture) : texture(texture) {}

};
