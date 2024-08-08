#pragma once

#include <glm/glm.hpp>
#include <utility>

#include "avalon/logic/Component.hpp"
#include "avalon/logic/Object.hpp"
#include "avalon/utils/AssetPool.hpp"


class RenderComponent : public Component {
public:

    enum class Shape {
        QUAD
    };

    Shape shape;
    Transform transform;
    Color color;
    bool isVisible;

    Ref<Sprite> spriteRef;


    RenderComponent(const Transform &transform, const Color color, Shape shape = Shape::QUAD,
                    bool isVisible = true) : transform(transform),
                                             color(color),
                                             spriteRef(CreateRef<Sprite>()),
                                             shape(shape),
                                             isVisible(isVisible) {}

    RenderComponent(const Transform &transform, const Ref<Sprite>& sprite, Shape shape = Shape::QUAD,
                    bool isVisible = true) : transform(transform),
                                             color(Color(1.0f, 1.0f, 1.0f, 1.0f)),
                                             spriteRef(sprite),
                                             shape(shape),
                                             isVisible(isVisible) {}

    ~RenderComponent() override = default;


    void start() override {

    }

    void update(float dt) override {

    }
};


