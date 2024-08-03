#pragma once

#include <glm/glm.hpp>

#include "avalon/logic/Component.hpp"
#include "avalon/logic/Transform.hpp"


class RenderComponent : public Component {
public:

    enum class Type {
        Quad,
        Circle,
        Text,
        Line
    };

    Transform transform;
    std::vector<float> vertexArray;
    std::vector<float> indexArray;
    glm::vec4 color;
    bool isVisible;


    RenderComponent(const Transform &transform, const glm::vec4 &color = {1.0f, 1.0f, 1.0f, 1.0f}, bool isVisible = true) : transform(transform),
                                                                                          color(color),
                                                                                          isVisible(isVisible) {}

    ~RenderComponent() override = default;

    void start() override {

    }

    void update(float dt) override {

    }
};


