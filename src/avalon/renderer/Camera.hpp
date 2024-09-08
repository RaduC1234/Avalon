#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

class Camera {
public:

    Camera() = default;

    Camera(const glm::vec2 &worldPosition, const glm::vec2 &worldSize = {1920, 1080}/*, bool worldCentered = false*/,
           const glm::vec2 &screenPosition = {0.0f, 0.0f}, const glm::vec2 &screenSize = {1.0f, 1.0f})
            : worldPosition(worldPosition), worldSize(worldSize),
              screenPosition(screenPosition), screenSize(screenSize) {
        adjustProjection();
    }


    void adjustProjection() {
        float left = worldPosition.x;
        float right = worldPosition.x + worldSize.x * screenSize.x;
        float bottom = worldPosition.y;
        float top = worldPosition.y + worldSize.y * screenSize.y;

        projectionMatrix = glm::ortho(left, right, bottom, top, 0.0f, 100.0f);
    }

    glm::mat4 getViewMatrix() {
        glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
        viewMatrix = glm::lookAt(
                glm::vec3(worldPosition.x, worldPosition.y, 20.0f),
                cameraFront + glm::vec3(worldPosition.x, worldPosition.y, 0.0f),
                cameraUp
        );

        return viewMatrix;
    }

    glm::mat4 getProjectionMatrix() const {
        return this->projectionMatrix;
    }

private:
    glm::mat4 projectionMatrix, viewMatrix;
    glm::vec2 worldPosition, worldSize;
    glm::vec2 screenPosition, screenSize;
};
