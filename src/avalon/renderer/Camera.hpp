#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

class Camera {
public:
    Camera() = default;

    Camera(const glm::vec2 &worldPosition, float zoom = 1.0f) : worldPosition(worldPosition), zoomFactor(zoom) {}

    void applyViewport(int windowWidth, int windowHeight, float desiredAspectRatio = 16.0f / 9.0f) {
        float viewportWidth, viewportHeight;

        // Crop horizontally if the desired aspect ratio < window
        if (static_cast<float>(windowWidth) / windowHeight > desiredAspectRatio) {
            viewportHeight = static_cast<float>(windowHeight);
            viewportWidth = viewportHeight * desiredAspectRatio;
        } else {
            // Crop vertically if the desired aspect ratio < window
            viewportWidth = static_cast<float>(windowWidth);
            viewportHeight = viewportWidth / desiredAspectRatio;
        }

        int xOffset = (windowWidth - static_cast<int>(viewportWidth)) / 2;
        int yOffset = (windowHeight - static_cast<int>(viewportHeight)) / 2;

        glViewport(xOffset, yOffset, static_cast<GLsizei>(viewportWidth), static_cast<GLsizei>(viewportHeight));

        float halfWidth = viewportWidth / this->zoomFactor * 0.5f;
        float halfHeight = viewportHeight / this->zoomFactor * 0.5f;

        float left = worldPosition.x - halfWidth;
        float right = worldPosition.x + halfWidth;
        float bottom = worldPosition.y - halfHeight;
        float top = worldPosition.y + halfHeight;

        this->projectionMatrix = glm::ortho(left, right, bottom, top, 0.0f, 100.0f);

       this->windowSize = glm::vec2(windowWidth, windowHeight);
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
        return projectionMatrix;
    }

    void setPosition(const glm::vec2 &position) {
        worldPosition = position;
    }

    void zoom(float factor) {
        zoomFactor *= factor;
    }

    void position(const glm::vec2 &offset) {
        worldPosition += offset;
    }

    glm::vec2 screenToWorld(const glm::vec2 &screenCoords) const {
        glm::vec2 normalizedScreen = glm::vec2(
                (2.0f * screenCoords.x) / windowSize.x - 1.0f,
                1.0f - (2.0f * screenCoords.y) / windowSize.y
        );

        glm::vec4 screenPos = glm::vec4(normalizedScreen, 0.0f, 1.0f);

        glm::mat4 inverseVP = glm::inverse(projectionMatrix * viewMatrix);

        glm::vec4 worldPos = inverseVP * screenPos;

        return glm::vec2(worldPos.x, worldPos.y);
    }

    glm::vec2 worldToScreen(const glm::vec2 &worldCoords) const {
        glm::vec4 worldPos = glm::vec4(worldCoords, 0.0f, 1.0f);

        glm::vec4 clipSpacePos = projectionMatrix * viewMatrix * worldPos;

        glm::vec3 ndcPos = glm::vec3(clipSpacePos) / clipSpacePos.w;

        glm::vec2 screenCoords = glm::vec2(
                (ndcPos.x + 1.0f) * 0.5f * windowSize.x,
                (1.0f - ndcPos.y) * 0.5f * windowSize.y
        );

        return screenCoords;
    }

private:
    glm::mat4 projectionMatrix, viewMatrix;
    glm::vec2 worldPosition;
    glm::vec2 windowSize;  // Store window size for screen to world conversion
    float zoomFactor;
};
