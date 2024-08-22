#pragma once

#include "avalon/logic/Component.hpp"
#include "avalon/logic/Object.hpp"
#include "avalon/utils/AssetPool.hpp"
#include "avalon/logic/Layer.hpp"

#include <glm/glm.hpp>

class RenderComponent : public Component {
public:
    enum class Shape {
        QUAD
    };

    glm::vec2 position;
    glm::vec2 scale;
    glm::vec4 color;
    Shape shape;
    Sprite sprite;
    bool isVisible;
    int zIndex = Layer::MID;

    RenderComponent(const glm::vec2 &position, const glm::vec2 &scale, const glm::vec4 &color, const Sprite &sprite,
                    int zIndex, Shape shape, bool isVisible = true) : position(position), scale(scale), color(color),
                                                               sprite(sprite), zIndex(zIndex), shape(shape),
                                                               isVisible(isVisible) {}
};


