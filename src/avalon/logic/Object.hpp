#pragma once

#include "avalon/core/Core.hpp"
#include "Component.hpp"
#include "Transform.hpp"
#include "avalon/components/RenderComponent.hpp"

/**
 * Usage with components:
 *
 *
 * @code
//Example components
class PositionComponent : public Component {
public:
    float x, y;

    PositionComponent(float x = 0, float y = 0) : x(x), y(y) {}
};

class VelocityComponent : public Component {
public:
    float vx, vy;

    VelocityComponent(float vx = 0, float vy = 0) : vx(vx), vy(vy) {}
};

int main() {
    Object obj;

    // Adding components
    obj.addComponent<PositionComponent>(10.0f, 20.0f);
    obj.addComponent<VelocityComponent>(1.0f, 1.5f);

    // Retrieving and using components
    PositionComponent* pos = obj.getComponent<PositionComponent>();
    VelocityComponent* vel = obj.getComponent<VelocityComponent>();

    if (pos) {
        std::cout << "Position: (" << pos->x << ", " << pos->y << ")\n";
    }

    if (vel) {
        std::cout << "Velocity: (" << vel->vx << ", " << vel->vy << ")\n";
    }

    // Removing a component
    obj.removeComponent<VelocityComponent>();

    // Check if the component was removed
    vel = obj.getComponent<VelocityComponent>();
    if (!vel) {
        std::cout << "Velocity component removed.\n";
    }

    return 0;
 *}
 *
 */
class Object {
    std::string name = "Object";
    std::unordered_map<std::type_index, Scope<Component>> components;

public:

    Object() = default;

    Object(std::string name, const Transform& transform) :  name(std::move(name)) {
        addComponent<RenderComponent>(transform);
    }

    Object(const Object &) = delete;
    Object &operator=(const Object &) = delete;
    Object(Object &&) noexcept = default;
    Object &operator=(Object &&) noexcept = default;

    /**
     * Adds an component to the object. Note that the duplicates are not allowed.
     */
    template<typename T, typename... Args>
    void addComponent(Args &&... args) {
        components[typeid(T)] = std::make_unique<T>(std::forward<Args>(args)...); // do not use CreateScope std::forward 2 times
    }

    template<typename T>
    T *getComponent() {
        auto it = components.find(typeid(T));

        if (it != components.end()) {
            return static_cast<T *>(it->second.get());
        }
        return nullptr;
    }

    template<typename T>
    void removeComponent() {
        components.erase(typeid(T));
    }

    void start() {
        for (auto &x: components) {
            x.second->start();
        }
    }

    void update(float dt) {
        for (auto &x: components) {
            x.second->update(dt);
        }
    }
};
