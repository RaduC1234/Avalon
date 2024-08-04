#pragma once

class Component {
public:
    virtual ~Component() {};

    virtual void start() = 0;
    virtual void update(float dt) = 0;
};
