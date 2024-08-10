#include "Application.hpp"

Application::Application() {

    Log::init();
    Time::init();

    AV_CORE_INFO("Starting application!");

    this->window = CreateScope<Window>("Avalon Window");

    changeScene(CreateScope<LevelEditorScene>());

    isRunning = true;
}


void Application::run() {
    float beginTime = Time::getTime();
    float endTime;
    float dt = -1.0f;


    while (isRunning) {

        if (dt >= 0 && currentScene != nullptr)
            this->currentScene->update(dt, window->getWidth(), window->getHeight());

        this->window->onUpdate();

        endTime = Time::getTime();
        dt = endTime - beginTime;
        beginTime = endTime;
    }
}

void Application::changeScene(Scope<Scene> scene) {
    this->currentScene = std::move(scene);
    currentScene->init();
    currentScene->start();
}
