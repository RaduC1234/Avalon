#pragma once

#include "avalon/scene/Component.hpp"
#include "avalon/entity/Entity.hpp"
#include "avalon/scene/Layer.hpp"
#include "avalon/utils/AssetPool.hpp"

#include <glm/glm.hpp>

class RenderComponent : public Component {
public:
    enum class Shape {
        QUAD
    };

    glm::vec3 position;
    glm::vec2 scale;
    glm::vec4 color;
    Shape shape;
    Sprite sprite;
    bool isVisible;

    RenderComponent(const glm::vec3 &position, const glm::vec2 &scale, const glm::vec4 &color, const Sprite &sprite, Shape shape, bool isVisible = true) : position(position), scale(scale), color(color),
                                                               sprite(sprite), shape(shape), isVisible(isVisible) {}
};


