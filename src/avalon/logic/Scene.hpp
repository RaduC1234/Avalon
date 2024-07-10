#ifndef AVALON_SCENE_HPP
#define AVALON_SCENE_HPP

#include "../logic/GameObject.hpp"
#include "../render/Camera.hpp"
#include "../render/Texture.hpp"
#include "../render/Shader.hpp"
#include "../render/Renderer.hpp"

class Scene {
protected:
    Camera camera;
    bool isRunning = false;
    std::vector<std::shared_ptr<GameObject>> objects;

public:
    Scene() {
        camera = Camera(-200, -300);
    }

    virtual ~Scene() = default;

    virtual void init() {}

    void start() {
        for (auto &x: objects) {
            //x->start();
        }
        isRunning = true;
    }

    void addGameObjectToScene(const std::shared_ptr<GameObject> &object) {
        if (!isRunning) {
            objects.push_back(object);
        } else {
            objects.push_back(object);
            //object->start();
        }
    }

    virtual void update(float deltaTime) = 0;

};

class LevelEditorScene : public Scene {

    GLuint VAO, VBO, EBO;
    Shader shader = Shader("resources/shaders/default.glsl");
    Texture texture = Texture("resources/textures/Brick_Wall.png");

    static constexpr float vertexArray[] = {
            // position                 // color            // uv
            400.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1, 0, // bottom right
            0.0f, 400.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0, 1, // top left
            400.0f, 400.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1, 1, // top right
            0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0, 0  // bottom left
    };

    // must be in counter-clockwise order
    static constexpr int elementArray[] = {
            2, 1, 0,
            0, 1, 3
    };

public:

    void init() override {
        //this->shader = Shader("resources/shaders/default.glsl");

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
        int uvSize = 2;
        int vertexSizeBytes = (positionsSize + colorSize + uvSize) * sizeof(float);

        // bind position on location 0
        glVertexAttribPointer(0, positionsSize, GL_FLOAT, GL_FALSE, vertexSizeBytes, (void *) 0);

        // bind color on position 1
        glVertexAttribPointer(1, colorSize, GL_FLOAT, GL_FALSE, vertexSizeBytes,
                              (void *) (positionsSize * sizeof(float)));

        // bind uv on position 2
        glVertexAttribPointer(2, uvSize, GL_FLOAT, GL_FALSE, vertexSizeBytes,
                              (void *) ((positionsSize + colorSize) * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
        glBindVertexArray(0); // Unbind the VAO

        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    }

        void update(float deltaTime) override {
            //camera.position.x -= deltaTime * 50.0f;

            shader.bind();

            // Upload Texture
            shader.uploadTexture("TEX_SAMPLER", 0);
            glActiveTexture(GL_TEXTURE0);
            texture.bind();


            shader.uploadMat4f("uProjection", camera.getProjectionMatrix());
            shader.uploadMat4f("uView", camera.getViewMatrix());

            shader.uploadFloat("uTime", TimeUtils::getTime());

            glBindVertexArray(VAO); // Bind the VAO

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw the triangles

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);

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
