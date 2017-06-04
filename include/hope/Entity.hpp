#pragma once

#include <bitset>

#include "Config.hpp"

namespace hope {

class Entity {
public:
    Entity() {

    }

    template<typename Component>
    void addComponent() {}

    template<typename Component>
    void removeComponent() {};

    void destroy() {};

private:
    std::bitset<MAX_COMPONENTS> _componentsMask;
    Id _id;
};
}