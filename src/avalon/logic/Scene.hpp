#ifndef AVALON_SCENE_HPP
#define AVALON_SCENE_HPP

#include "../renderer/Camera.hpp"
#include "../renderer/Texture.hpp"
#include "../renderer/Shader.hpp"

#include "Object.hpp"

class Scene {
protected:
    Camera camera;
    bool isRunning = false;
    std::vector<Object> objects;

public:
    Scene() {
        camera = Camera(-200, -300);
    }

    virtual ~Scene() = default;

    virtual void init() {}

    void start() {
        for (auto &x: objects) {
            x.start();
        }
    }

    virtual void update(float deltaTime) = 0;

    void addObject(Object obj) {
        objects.push_back(std::move(obj));
    }

    void removeObject(std::string name) {

    }
};

class LevelEditorScene : public Scene {

    GLuint VAO, VBO, EBO;
    Shader shader = Shader("resources/shaders/default.glsl");

    static constexpr float vertexArray[] = {
            // First quad vertices
            // position              // color
            400.0f, 0.0f, 0.0f,     1.0f, 0.0f, 1.0f, 1.0f, // bottom right
            0.0f, 400.0f, 0.0f,     1.0f, 0.0f, 1.0f, 1.0f, // top left
            400.0f, 400.0f, 0.0f,   1.0f, 0.0f, 1.0f, 1.0f, // top right
            0.0f, 0.0f, 0.0f,       1.0f, 0.0f, 1.0f, 1.0f, // bottom left

            // Second quad vertices
            // position              // color
            1200.0f, 0.0f, 0.0f,     0.0f, 1.0f, 1.0f, 1.0f, // bottom right
            800.0f, 400.0f, 0.0f,    0.0f, 1.0f, 1.0f, 1.0f, // top left
            1200.0f, 400.0f, 0.0f,   0.0f, 1.0f, 1.0f, 1.0f, // top right
            800.0f, 0.0f, 0.0f,      0.0f, 1.0f, 1.0f, 1.0f  // bottom left
    };

    // must be in counter-clockwise order
    static constexpr int elementArray[] = {
            // First quad indices
            2, 1, 0,
            0, 1, 3,

            // Second quad indices (adjusted for the second set of vertices)
            6, 5, 4,
            4, 5, 7
    };

public:
    void init() override {
        // Create and bind the Vertex Array Object (VAO)
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Generate and bind the Vertex Buffer Object (VBO)
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

        // Generate and bind the Element Buffer Object (EBO)
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementArray), elementArray, GL_STATIC_DRAW);

        int positionSize = 3; // x, y, z
        int colorSize = 4; // r, g, b, a
        int vertexSizeBytes = (positionSize + colorSize) * sizeof(float);

        // bind position on location 0
        glVertexAttribPointer(0, positionSize, GL_FLOAT, GL_FALSE, vertexSizeBytes, (void *) 0);
        glEnableVertexAttribArray(0);

        // bind color on location 1
        glVertexAttribPointer(1, colorSize, GL_FLOAT, GL_FALSE, vertexSizeBytes,
                              (void *) (positionSize * sizeof(float)));
        glEnableVertexAttribArray(1);



        glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
        glBindVertexArray(0); // Unbind the VAO

        start();

        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }

    void update(float deltaTime) override {
        if (KEY_PRESSED(GLFW_KEY_D))
            camera.position.x -= deltaTime * 200.0f;

        if (KEY_PRESSED(GLFW_KEY_A))
            camera.position.x += deltaTime * 200.0f;

        if (KEY_PRESSED(GLFW_KEY_W))
            camera.position.y -= deltaTime * 200.0f;

        if (KEY_PRESSED(GLFW_KEY_S))
            camera.position.y += deltaTime * 200.0f;

        shader.bind();

        shader.uploadMat4f("uProjection", camera.getProjectionMatrix());
        shader.uploadMat4f("uView", camera.getViewMatrix());
        shader.uploadFloat("uTime", Time::getTime());

        glBindVertexArray(VAO); // Bind the VAO

        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0); // Draw the triangles for both quads

        glBindVertexArray(0); // Unbind the VAO

        for (auto &x : objects) {
            x.update(deltaTime);
        }
    }
};


class LevelScene : public Scene {
public:

    void init() override {

    }

    void update(float deltaTime) override {

    }
};

#endif //AVALON_SCENE_HPP
