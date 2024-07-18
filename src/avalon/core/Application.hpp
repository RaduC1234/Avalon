#pragma once

#include "Core.hpp"
#include "Window.hpp"
#include "avalon/logic/Scene.hpp"

class Application {
private:
    Scope<Window> window;
    Scope<Scene> currentScene;

    bool isRunning = true;
    bool isMinimized = false;
public:

    Application();

    // Destructor
    ~Application() = default;

    // Accessor for window
    Window& getWindow() { return *this->window; }

    // Run the application
    void run();

    // Change the current scene
    void changeScene(Scope<Scene> scene);
};
