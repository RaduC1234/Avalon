#pragma once

#include "avalon/scene/Scene.hpp"
#include "avalon/imgui/ImGuiLayer.hpp"

class LevelEditorScene : public Scene {
public:

    explicit LevelEditorScene(GLFWwindow *glfWwindow) : Scene(glfWwindow) {}

    void onCreate() override {

        layers.pushOverlay(CreateScope<ImGuiLayer>(this->glfWwindow));

        this->camera = CreateRef<Camera>(-200, -300);
        this->renderer = Renderer(1000, camera);

        AssetPool::loadIndexedResources();
    }

    void onStart() override {

    }

    void onUpdate(float deltaTime) override {

        for(auto x : layers) {
            x->onUpdate(deltaTime);
        }

        if (KEY_PRESSED(GLFW_KEY_D))
            camera->position.x += deltaTime * 200.0f;

        if (KEY_PRESSED(GLFW_KEY_A))
            camera->position.x -= deltaTime * 200.0f;

        if (KEY_PRESSED(GLFW_KEY_W))
            camera->position.y += deltaTime * 200.0f;

        if (KEY_PRESSED(GLFW_KEY_S))
            camera->position.y -= deltaTime * 200.0f;

        renderer.drawQuad({100, 100, 1}, {100, 100}, Color(1.0f, 0.0f, 1.0f, 0.5f));

        renderer.drawQuad({150, 0, 2}, {100, 100},  Color(210, 109, 101, 150)); // red
        renderer.drawQuad({225, 0, 1}, {100, 100},  Color(136, 193, 99, 50)); // green
        renderer.drawQuad({175, 50,3}, {100, 100},  Color(17, 33, 94, 100)); // blue
    }

    void onRender(int screenWidth, int screenHeight) override {
        renderer.flush();

        for(auto x : layers) {
            x->onImGuiRender();
        }
    }

    void onDestroy() override {

    }
};

