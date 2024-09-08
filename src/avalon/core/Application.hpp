#pragma once

#include "Core.hpp"
#include "Window.hpp"
#include "ImGuiLayer.hpp"

struct ApplicationSpecification {
    std::string name = "Avalon Window";
};

class Application {
public:

    Application(const ApplicationSpecification &specification = {"Avalon Window"});

    ~Application() = default;

    static Application *getInstance() {
        return instance;
    }

    Window &getWindow() { return *this->window; }

    void run();

    void changeScene(Scope<Scene> scene);

private:

    Scope<Window> window;
    Scope<Scene> currentScene;
    ImGuiLayer imGuiLayer;

    ApplicationSpecification specification;
    bool isRunning = true;
    bool isMinimized = false;

    static Application *instance;
};

Application *createApplication();
