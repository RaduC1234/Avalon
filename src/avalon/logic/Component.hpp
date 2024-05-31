#ifndef AVALON_COMPONENT_HPP
#define AVALON_COMPONENT_HPP

class Component {
public:
    virtual ~Component() = default;
    virtual void start() = 0;
};

#endif
