#ifndef AVALON_SCENE_HPP
#define AVALON_SCENE_HPP

#include "Object.hpp"
#include "avalon/renderer/Renderer.hpp"

class Scene {
protected:
    Ref<Camera> camera;
    Renderer renderer;
    std::vector<Object> objects;
    bool isRunning = false;


public:
    Scene() {
        camera = CreateRef<Camera>(-200, -300);
        renderer = Renderer(2, camera);
    }

    virtual ~Scene() = default;

    virtual void init() {}

    void start() {
        for (auto &x: objects) {
            x.start();
        }
    }

    virtual void update(float deltaTime) = 0;

    void addObject(Object &obj) {
        objects.push_back(std::move(obj));
    }

    void removeObject(std::string name) {

    }
};

class LevelEditorScene : public Scene {
public:

    void init() override {

        AssetPool::loadIndexedResources();

        Object obj1("Obj1", Transform(150, 0, 100, 100), Color(210, 109, 101, 150), 2); // red
        Object obj2("Obj1", Transform(225, 0, 100, 100),  Color(136, 193, 99, 50), 1); // green
        Object obj3("Obj2", Transform(175, 50, 100, 100), Color(17, 33, 94, 100), 3); // blue

        addObject(obj1);
        addObject(obj2);
        addObject(obj3);

/*        int i = 0;
        for(SpriteSheet x : AssetPool::spriteSheets) {
            for(Sprite y : x.sprites) {
                Object obj("onj", Transform(i++ * 70, 0, 64, 64), y);
                addObject(obj);
            }
        }*/
    }

    void update(float deltaTime) override {
        if (KEY_PRESSED(GLFW_KEY_D))
            camera->position.x += deltaTime * 200.0f;

        if (KEY_PRESSED(GLFW_KEY_A))
            camera->position.x -= deltaTime * 200.0f;

        if (KEY_PRESSED(GLFW_KEY_W))
            camera->position.y += deltaTime * 200.0f;

        if (KEY_PRESSED(GLFW_KEY_S))
            camera->position.y -= deltaTime * 200.0f;

        renderer.addAll(objects);
        renderer.flush();

        for (auto &x: objects) {
            x.update(deltaTime);
        }
    }
};


class LevelScene : public Scene {
public:

    void init() override {

    }

    void update(float deltaTime) override {

    }
};

#endif //AVALON_SCENE_HPP
