#include "Application.hpp"

Application::Application(const ApplicationSpecification& specification) : specification(specification) {

    instance = this;

    Log::init();

    AV_CORE_INFO("Starting application!");

    this->window = CreateScope<Window>(specification.name);

    isRunning = true;
}

void Application::run() {
    float beginTime = Time::getTime();
    float endTime;
    float dt = -1.0f;

    while (isRunning) {
        // todo: double buffering
        if (dt >= 0 && currentScene != nullptr) {
            this->currentScene->onUpdate(dt);
            this->currentScene->onRender(window->getWidth(), window->getHeight());
        }

        this->window->onUpdate();

        endTime = Time::getTime();
        dt = endTime - beginTime;
        beginTime = endTime;
    }

    AssetPool::unloadAll();
}

void Application::changeScene(Scope<Scene> scene) {
    if (currentScene != nullptr)
        this->currentScene->onDestroy();

    this->currentScene = std::move(scene);
    currentScene->onCreate();
    currentScene->onStart();
}

Application* Application::instance = nullptr;
