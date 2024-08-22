#pragma once

#include "Component.hpp"

#include "avalon/core/Core.hpp"

#include "avalon/renderer/Color.hpp"
#include "avalon/renderer/Transform.hpp"

#include "avalon/components/RenderComponent.hpp"


class Object {
    std::string name = "Object";
    std::unordered_map<std::type_index, Scope<Component>> components;

public:

    Object() = default;

    Object(std::string name, const Transform &transform, Color color, int zIndex) : name(std::move(name)) {
    }

    Object(std::string name, const Transform &transform, /*const Sprite &sprite,*/ int zIndex) : name(std::move(name)) {
    }

    Object(Object &&other) noexcept: name(std::move(other.name)), components(std::move(other.components)) {}

    /**
     * Adds an component to the object. Note that the duplicates are not allowed.
     */
    template<typename T, typename... Args>
    void addComponent(Args &&... args) {

        auto component = CreateScope<T>(std::forward<Args>(args)...);
        component->attachToObject(this);
        components[typeid(T)] = std::move(component);
    }

    template<typename T>
    T *getComponent() const {
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
};
