#ifndef AVALON_TRANSFORM_HPP
#define AVALON_TRANSFORM_HPP

#include <glm/glm.hpp>

typedef glm::vec4 Color;

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


#endif //AVALON_TRANSFORM_HPP
