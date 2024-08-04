#pragma once

#include <glm/glm.hpp>

#include "avalon/logic/Component.hpp"
#include "avalon/logic/Transform.hpp"


class RenderComponent : public Component {
public:

    enum class Shape {
        QUAD
    };

    Shape shape;
    Transform transform;
    //Texture texture;
    glm::vec4 color;
    bool isVisible;


    RenderComponent(const Transform &transform, const glm::vec4 &color = {1.0f, 1.0f, 1.0f, 1.0f}, Shape shape = Shape::QUAD, bool isVisible = true) : transform(transform),
                                                                                          color(color),
                                                                                          shape(shape),
                                                                                          isVisible(isVisible) {}

    ~RenderComponent() override = default;

    void start() override {

    }

    void update(float dt) override {

    }
};


