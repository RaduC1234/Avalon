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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    while (isRunning) {
        glClearColor(0.0863f, 0.0863f, 0.0863f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (dt >= 0 && currentScene != nullptr)
            this->currentScene->update(dt);

        this->window->onUpdate();


        endTime = Time::getTime();
        dt = endTime - beginTime;
        beginTime = endTime;
    }

    GLenum err;
    if ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
}

void Application::changeScene(Scope<Scene> scene) {
    this->currentScene = std::move(scene);
    currentScene->init();
    currentScene->start();
}
