#ifndef AVALON_GAMECOMPONENTS_HPP
#define AVALON_GAMECOMPONENTS_HPP

#include <string>
#include <memory>
#include <utility>
#include <vector>

#include "Component.hpp"

class GameObject {
    std::string name;
    std::vector<std::shared_ptr<Component>> components;

public:
    explicit GameObject(std::string name) : name(std::move(name)) {}

    void start() {

    }

public:

};


#endif //AVALON_GAMECOMPONENTS_HPP
