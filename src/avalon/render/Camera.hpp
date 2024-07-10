#ifndef AVALON_CAMERA_HPP
#define AVALON_CAMERA_HPP

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

#define TILES_HORIZONTALLY 40.0f
#define TILES_VERTICALLY 21.0f

#define TILE_SIZE 32.0f // in pixels

class Camera {
public:
    glm::mat4 projectionMatrix{}, viewMatrix{};
    glm::vec2 position{};


    Camera() = default;

    Camera(const float x, const float y) {
        position.x = x;
        position.y = y;
        adjustProjection();
    }

    explicit Camera(const glm::vec2 &position) : position(position) {
        adjustProjection();
    }


    void adjustProjection() {      // left,         right,                 bottom,        top,                near,    far
        projectionMatrix = glm::ortho(0.0f, TILE_SIZE * TILES_HORIZONTALLY, 0.0f, TILE_SIZE * TILES_VERTICALLY, 0.0f,100.0f);
    }

    glm::mat4 getViewMatrix() {
        glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
        viewMatrix = glm::lookAt(
                glm::vec3(position.x, position.y, 20.0f),
                cameraFront + glm::vec3(position.x, position.y, 0.0f),
                cameraUp
        );

        return viewMatrix;
    }

    glm::mat4 getProjectionMatrix() const {
        return this->projectionMatrix;
    }
};

#endif //AVALON_CAMERA_HPP
