#pragma once

class Component {
public:
    virtual ~Component() = 0;

    virtual void start() = 0;
    virtual void update(float dt) = 0;
};
