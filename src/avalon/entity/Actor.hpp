#pragma once

#include "Component.hpp"

using ActorId = std::size_t;

class Registry;

class Actor {
public:

    Actor(ActorId id, Registry* registry) : id(id), parentRegistry(registry) {}

    /**
     * Adds an component to the object. Note that the duplicates are not allowed.
     */
    template<typename T, typename... Args>
    void addComponent(Args &&... args);

    template<typename T>
    T *getComponent() const;

    template<typename T>
    void removeComponent();

    ActorId getId() const {
        return this->id;
    }

    bool isValid() const {
        return parentRegistry != nullptr;
    }

    friend void to_json(json &j, const Actor& actor);

    friend void from_json(const json *j, Actor& actor);

private:
    ActorId id;
    Registry* parentRegistry;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
};

namespace std {
    template<>
    struct hash<Actor> {
        std::size_t operator()(const Actor& actor) const noexcept {
            return std::hash<int>()(actor.getId());
        }
    };
}
