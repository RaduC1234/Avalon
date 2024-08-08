#include "Application.hpp"

Application::Application() {

    Log::init();
    Time::init();

    AV_CORE_INFO("Starting application!");

    this->window = CreateScope<Window>("Avalon Window");

    changeScene(CreateScope<LevelEditorScene>());

    int textureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
    AV_CORE_INFO("Texture units available on hardware: {0}.", textureUnits);


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

        GLenum err;
        if ((err = glGetError()) != GL_NO_ERROR) {
            AV_CORE_ERROR("OpenGL error: {0}", err);
        }

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
