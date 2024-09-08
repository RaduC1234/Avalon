#pragma once

#include "Avalon.hpp"
#include "avalon/scene/ImGuiLayer.hpp"

class LevelScene : public Scene {

public:
    void onCreate() override {

        layers.pushOverlay(CreateScope<ImGuiLayer>());

        this->levelCamera = Camera({-200, -300});
        this->renderer = Renderer(1000);
    }

    void onStart() override {

    }

    void onUpdate(float deltaTime) override {

        renderer.drawQuad({100, 100, 1}, {100, 100}, Color(1.0f, 0.0f, 1.0f, 0.5f));

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
