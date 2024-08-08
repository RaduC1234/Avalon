#pragma once

#include <glm/glm.hpp>

class Transform {
public:

    glm::vec2 position{0, 0};
    glm::vec2 scale{1, 1};

    Transform() = default;

    Transform(float x, float y, float u, float v) {
        position = glm::vec2(x, y);
        scale = glm::vec2(u, v);
    }

    Transform(glm::vec2 position, glm::vec2 scale) {
        this->position = position;
        this->scale = scale;
    }
};
