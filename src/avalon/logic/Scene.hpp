#ifndef AVALON_SCENE_HPP
#define AVALON_SCENE_HPP

#include "../renderer/Camera.hpp"
#include "../renderer/Texture.hpp"
#include "../renderer/Shader.hpp"

#include "Object.hpp"
#include "avalon/renderer/Renderer.hpp"

class Scene {
protected:
    Ref<Camera> camera;
    Renderer renderer;
    std::vector<Object> objects;
    bool isRunning = false;


public:
    Scene() {
        camera = CreateRef<Camera>(-200, -300);
        renderer = Renderer(1000, camera);
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

public:

    void init() override {
        renderer.start();

    }

    void update(float deltaTime) override {
        if (KEY_PRESSED(GLFW_KEY_D))
            camera->position.x += deltaTime * 200.0f;

        if (KEY_PRESSED(GLFW_KEY_A))
            camera->position.x -= deltaTime * 200.0f;

        if (KEY_PRESSED(GLFW_KEY_W))
            camera->position.y += deltaTime * 200.0f;

        if (KEY_PRESSED(GLFW_KEY_S))
            camera->position.y -= deltaTime * 200.0f;

        //renderer.registerObjects(this->objects);
        renderer.render();
        //renderer.flush();


        for (auto &x: objects) {
            x.update(deltaTime);
        }

        AV_CORE_INFO(camera->position.x);
        AV_CORE_INFO(camera->position.y);
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
