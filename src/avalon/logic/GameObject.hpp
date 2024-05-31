#ifndef AVALON_GAMEOBJECT_HPP
#define AVALON_GAMEOBJECT_HPP

#include <string>
#include <memory>
#include <utility>
#include <vector>

#include "Component.hpp"

class GameObject {
private:
    std::string name;
    std::vector<std::shared_ptr<Component>> components;

public:
    explicit GameObject(std::string name) : name(std::move(name)) {}

    void start() {
        for (const auto& x : components) {
            x->start();
        }
    }
};

#endif //AVALON_GAMEOBJECT_HPP
