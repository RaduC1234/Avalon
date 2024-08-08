#pragma once

#include "avalon/core/Core.hpp"

class Object;

class Component {
protected:
    Object* parent = nullptr;

public:
    virtual ~Component() {};

    virtual void attachToObject(Object* object) {
        this->parent = object;
    }

    virtual void start() = 0;
    virtual void update(float dt) = 0;
};
