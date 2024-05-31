#ifndef AVALON_SCENE_HPP
#define AVALON_SCENE_HPP

#include "Component.hpp"
#include "../logic/Scene.hpp"
#include "../logic/GameObject.hpp"
#include "../render/Camera.hpp"

class Scene {
protected:
    std::unique_ptr<Camera> camera;
    bool isRunning = false;
    std::vector<std::shared_ptr<GameObject>> objects;

public:
    Scene() : camera(std::make_unique<Camera>()) {

    }

    virtual ~Scene() = default;

    virtual void init() {}

    void start() {
        for (auto &x: objects) {
            x->start();
        }
        isRunning = true;
    }

    void addGameObjectToScene(const std::shared_ptr<GameObject> &object) {
        if (!isRunning) {
            objects.push_back(object);
        } else {
            objects.push_back(object);
            object->start();
        }
    }

    virtual void update(float deltaTime) = 0;

};

class LevelEditorScene : public Scene {

    Shader *shader;
    GLuint VAO, VBO, EBO;

    static constexpr float vertexArray[] = {
            // position                 // color
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // bottom right
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top left
            0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f  // bottom left
    };

    // must be in counter-clockwise order
    static constexpr int elementArray[] = {
            2, 1, 0,
            0, 1, 3
    };

public:

    ~LevelEditorScene() override {
        delete shader;
    }

    void init() override {
        this->shader = new Shader("resources/shaders/default.glsl");


        // Copy-pasting memory from RAM to VRAM
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

        int positionsSize = 3; // x, y, z
        int colorSize = 4; // r, b, g, w
        int vertexSizeBytes = (positionsSize + colorSize) * sizeof(float);

        // bind position on location 0
        glVertexAttribPointer(0, positionsSize, GL_FLOAT, GL_FALSE, vertexSizeBytes, (void *) 0);


        // bind color on position 1
        glVertexAttribPointer(1, colorSize, GL_FLOAT, GL_FALSE, vertexSizeBytes, (void*)(positionsSize * sizeof(float)));


        glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
        glBindVertexArray(0); // Unbind the VAO

        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    }

    void update(float deltaTime) override {

        shader->bind();

        glBindVertexArray(VAO); // Bind the VAO

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw the triangles

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glBindVertexArray(0); // Unbind the VAO
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
