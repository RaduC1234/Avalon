#ifndef AVALON_SCENE_HPP
#define AVALON_SCENE_HPP

#include <vector>
#include <memory>

#include "GameObject.hpp"
#include "../logic/Scene.hpp"
#include "../render/Camera.hpp"

class Scene {
protected:
    std::unique_ptr<Camera> camera;
    bool isRunning = false;
    std::vector<std::shared_ptr<GameObject>> objects;

public:
    Scene() : camera(std::make_unique<Camera>()) {}

    virtual ~Scene() = default; // Virtual destructor for proper cleanup

    virtual void init() = 0;

    void start() {
        for (auto &go: objects) {
            go->start();
        }
        isRunning = true;
    }

    void addGameObjectToScene(const std::shared_ptr<GameObject> &go) {
        if (!isRunning) {
            objects.push_back(go);
        } else {
            objects.push_back(go);
            go->start();
        }
    }

    virtual void update(float dt) = 0; // Pure virtual function to be implemented by derived classes

};

class LevelEditorScene : public Scene {

public:
    void init() override {

    }

    void update(float dt) override {

    }
};

class LevelScene : public Scene {
public:

    void init() override {

    }

    void update(float dt) override {

    }
};

#endif //AVALON_SCENE_HPP
