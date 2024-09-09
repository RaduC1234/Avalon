#pragma once

#include "avalon/core/Core.hpp"

#include "Shader.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "Texture.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

using TextureCoords = std::array<glm::vec2, 4>;

enum RenderType {
    QUAD,
    TEXT
};

class RenderBatch {
public:

    RenderBatch(uint32_t maxBatchSize, uint32_t zIndex, RenderType batchType) : maxBatchSize(maxBatchSize), zIndex(zIndex), batchType(batchType) {}

    virtual ~RenderBatch() = default;

    virtual void start() = 0;

    virtual void render(uint32_t screenWidth, uint32_t screenHeight, Camera &camera) = 0;

    uint32_t getZIndex() const { return this->zIndex; }

    uint32_t getMaxBatchSize() const {
        return maxBatchSize;
    }

    RenderType getBatchType() const {
        return batchType;
    }

protected:
    uint32_t maxBatchSize;
    uint32_t zIndex;
    RenderType batchType;
};
