#pragma once

#include "avalon/renderer/Renderer.hpp"

#include <GLFW/glfw3.h>

struct SceneContext {
    GLFWwindow *nativeWindow;
    std::string &name;
};

class Scene {
public:

    explicit Scene(GLFWwindow *glfWwindow) : glfWwindow(glfWwindow) {}

    virtual ~Scene() = default;

    virtual void onCreate() =0;

    virtual void onStart() = 0;

    virtual void onUpdate(float deltaTime) = 0;

    virtual void onRender(int screenWidth, int screenHeight) = 0;

    virtual void onDestroy() = 0;

protected:

    GLFWwindow* glfWwindow;
    Ref<Camera> camera;
    Renderer renderer;
    LayerStack layers;
};
