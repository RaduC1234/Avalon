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

    Shape shape;
    Transform transform;
    Color color;
    bool isVisible;
    int zIndex = Layer::MID;

    Sprite sprite;


    RenderComponent(const Transform &transform, const Color color, int zIndex = Layer::MID, Shape shape = Shape::QUAD,
                    bool isVisible = true) : transform(transform),
                                             color(color),
                                             sprite(Sprite()),
                                             shape(shape),
                                             zIndex(zIndex),
                                             isVisible(isVisible) {}

    RenderComponent(const Transform &transform, Sprite sprite, int zIndex = Layer::MID, Shape shape = Shape::QUAD,
                    bool isVisible = true) : transform(transform),
                                             color(Color(1.0f, 1.0f, 1.0f, 1.0f)),
                                             sprite(std::move(sprite)),
                                             shape(shape),
                                             zIndex(zIndex),
                                             isVisible(isVisible) {}

    ~RenderComponent() override = default;


    // for multiset in renderer
    bool operator<(const RenderComponent& other) const {
        return this->zIndex > other.zIndex;
    }
};


