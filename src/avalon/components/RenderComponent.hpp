#pragma once

#include <glm/glm.hpp>
#include <utility>

#include "avalon/logic/Component.hpp"
#include "avalon/logic/Object.hpp"
#include "avalon/logic/AssetPool.hpp"


class RenderComponent : public Component {
public:

    enum class Shape {
        QUAD
    };

    Shape shape;
    Transform transform;
    Color color;
    bool isVisible;

    Ref<Texture> textureRef;

    std::array<glm::vec2, 4> texCoords = {
            glm::vec2(1, 1),
            glm::vec2(1, 0),
            glm::vec2(0, 0),
            glm::vec2(0, 1)
    };


    RenderComponent(const Transform &transform, const Color color, Shape shape = Shape::QUAD,
                    bool isVisible = true) : transform(transform),
                                             color(color),
                                             textureRef(nullptr),
                                             shape(shape),
                                             isVisible(isVisible) {}

    RenderComponent(const Transform &transform, const Ref<Texture>& texture, Shape shape = Shape::QUAD,
                    bool isVisible = true) : transform(transform),
                                             color(Color(1.0f, 1.0f, 1.0f, 1.0f)),
                                             textureRef(texture),
                                             shape(shape),
                                             isVisible(isVisible) {}

    ~RenderComponent() override = default;


    void start() override {

    }

    void update(float dt) override {

    }
};


