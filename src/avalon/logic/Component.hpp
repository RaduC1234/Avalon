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
};
