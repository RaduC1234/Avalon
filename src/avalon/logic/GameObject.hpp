#ifndef AVALON_GAMEOBJECT_HPP
#define AVALON_GAMEOBJECT_HPP

#include "Transform.hpp"
#include "../render/ObjectRenderer.hpp"


class GameObject;
class Component;

class Component {
public:
    virtual ~Component() = default;
    void start() {}
    virtual void update(float dt) = 0;

protected:
    GameObject* object = nullptr;

    friend class GameObject;
};

class GameObject {
private:
    std::string name;
    std::vector<Component> components;

    // default "components"
    Transform transform{};
    ObjectRenderer renderer{};

public:
    explicit GameObject(std::string name) : name(std::move(name)) {}

    GameObject(std::string name, const Transform &transform) : name(std::move(name)), transform(transform) {}

};

#endif // AVALON_GAMEOBJECT_HPP
