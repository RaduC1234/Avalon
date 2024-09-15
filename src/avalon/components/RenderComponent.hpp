#pragma once

#include "avalon/entity/Component.hpp"
#include "avalon/entity/Actor.hpp"
#include "avalon/scene/Layer.hpp"
#include "avalon/utils/AssetPool.hpp"

#include <glm/glm.hpp>

enum Shape {
    QUAD,
    TEXT
};

NLOHMANN_JSON_SERIALIZE_ENUM( Shape, {
    {QUAD, "quad"}
})

class RenderComponent : public Component {
public:

    glm::vec3 position;
    glm::vec2 scale;
    glm::vec4 color;
    Shape shape;
    Sprite sprite;
    bool normalized;
    bool isVisible;

    RenderComponent(const glm::vec3 &position, const glm::vec2 &scale, const glm::vec4 &color, const Sprite &sprite, Shape shape, bool isVisible = true) : position(position), scale(scale), color(color),
                                                               sprite(sprite), shape(shape), isVisible(isVisible) {}
};


