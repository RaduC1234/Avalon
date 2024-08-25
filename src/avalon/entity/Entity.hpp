#pragma once

using EntityID = std::size_t;

class Registry;

class Entity {
public:
    Entity(Registry* registry, EntityID id) {

    }

    EntityID getId() {
        return 0;
    }

    bool isValid() {
        return false;
    }

    void destroy() {

    }

    template <typename T>
    void addComponent(T component) {

    }

    template <typename T>
    bool hasComponent() {
        return 0;
    }

    template<typename T>
    void removeComponent() {

    }

private:
    Registry* parentRegistry;
};
