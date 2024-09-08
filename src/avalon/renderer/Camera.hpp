#pragma once

#include <glm/glm.hpp>
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

        // Center the viewport within the window
        int xOffset = (windowWidth - static_cast<int>(viewportWidth)) / 2;
        int yOffset = (windowHeight - static_cast<int>(viewportHeight)) / 2;

        glViewport(xOffset, yOffset, static_cast<GLsizei>(viewportWidth), static_cast<GLsizei>(viewportHeight));
        //glViewport(0, 0, static_cast<GLsizei>(windowWidth), static_cast<GLsizei>(windowHeight));

        float halfWidth = viewportWidth / this->zoomFactor * 0.5f;
        float halfHeight = viewportHeight / this->zoomFactor * 0.5f;

        float left = worldPosition.x - halfWidth;
        float right = worldPosition.x + halfWidth;
        float bottom = worldPosition.y - halfHeight;
        float top = worldPosition.y + halfHeight;

        // Update the projection matrix
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

private:
    glm::mat4 projectionMatrix, viewMatrix;
    glm::vec2 worldPosition;
    float zoomFactor;
};
