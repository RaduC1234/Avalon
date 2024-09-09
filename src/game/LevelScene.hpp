#pragma once

#include "Avalon.hpp"

class LevelScene : public Scene {

public:
    void onCreate() override {

        this->levelCamera = Camera({0, 0}, 2.0f);
        this->renderer = Renderer(1000);
    }

    void onStart() override {

    }

    void onUpdate(float deltaTime) override {

        if (KEY_PRESSED(GLFW_KEY_D))
            levelCamera.position({deltaTime * 200.0f, 0});

        if (KEY_PRESSED(GLFW_KEY_A))
            levelCamera.position({-deltaTime * 200.0f, 0});

        if (KEY_PRESSED(GLFW_KEY_W))
            levelCamera.position({0, deltaTime * 200.0f});

        if (KEY_PRESSED(GLFW_KEY_S))
            levelCamera.position({0, -deltaTime * 200.0f});


        renderer.drawNormalizedQuad({0.0f, 0.0f, 1}, {1.0f, 1.0f}, Color(1.0f, 0.0f, 1.0f, 0.5f));

        renderer.drawQuad({150, 0, 2}, {100, 100},  Color(210, 109, 101, 150)); // red
        renderer.drawQuad({225, 0, 1}, {100, 100},  Color(136, 193, 99, 50)); // green
        renderer.drawQuad({175, 50,3}, {100, 100},  Color(17, 33, 94, 100)); // blue

        for(auto x : layers) {
            x->onUpdate(deltaTime);
        }
    }

    void onRender(int screenWidth, int screenHeight) override {
        renderer.flush(screenWidth, screenHeight, levelCamera);

        for(auto x : layers) {
            x->onImGuiRender();
        }
    }

    void onDestroy() override {

    }

private:
    Camera levelCamera;
};
