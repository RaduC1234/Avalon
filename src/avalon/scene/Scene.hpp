#pragma once

#include "avalon/entity/Registry.hpp"
#include "avalon/renderer/Renderer.hpp"

#include <GLFW/glfw3.h>

class Scene {
public:

    Scene() = default;

    virtual ~Scene() = default;

    virtual void onCreate() =0;

    virtual void onStart() = 0;

    virtual void onUpdate(float deltaTime) = 0;

    virtual void onRender(int screenWidth, int screenHeight) = 0;

    virtual void onDestroy() = 0;

protected:
    Camera camera;
    Renderer renderer;

    LayerStack layers;
    Registry registry;
    ResourceBundle *sceneBundle;

    friend class ImGuiLayer;
    friend class SceneInitializer;
};
