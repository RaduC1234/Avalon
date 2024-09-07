#include "Actor.hpp"

template<typename T, typename... Args>
void Actor::addComponent(Args &&... args) {
    components[typeid(T)] = std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
T *Actor::getComponent() const {
    auto it = components.find(typeid(T));

    if (it != components.end()) {
        return static_cast<T *>(it->second.get());
    }
    return nullptr;
}

template<typename T>
void Actor::removeComponent() {
    components.erase(typeid(T));
}
