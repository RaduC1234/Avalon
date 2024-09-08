#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

class Camera {
public:
    Camera() = default;

    Camera(const glm::vec2 &worldPosition, float zoom = 1.0f) : worldPosition(worldPosition), zoomFactor(zoom) {}

    /*
     * You don't t need to call this.
     */
    void applyViewport(int windowWidth, int windowHeight) {

        // Determine the size of the viewport to maintain a 1:1 aspect ratio
        float viewportSize = std::min(windowWidth, windowHeight);

        // Center the viewport within the window
        int xOffset = (windowWidth - viewportSize) / 2;
        int yOffset = (windowHeight - viewportSize) / 2;

        // Set the OpenGL viewport
        glViewport(xOffset, yOffset, static_cast<GLsizei>(viewportSize), static_cast<GLsizei>(viewportSize));

        // Adjust the projection matrix for a 1:1 aspect ratio
        float aspectRatio = 1.0f; // Fixed 1:1 aspect ratio
        float halfWidth = (viewportSize * zoomFactor) * 0.5f;
        float halfHeight = halfWidth / aspectRatio;

        float left = worldPosition.x - halfWidth;
        float right = worldPosition.x + halfWidth;
        float bottom = worldPosition.y - halfHeight;
        float top = worldPosition.y + halfHeight;

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
