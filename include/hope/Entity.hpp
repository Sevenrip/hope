#pragma once

#include <cassert>
#include <bitset>

#include "Config.hpp"
#include "utils/IdGenerator.hpp"
#include <iostream>

namespace hope {

struct ComponentTag {};

template<typename T>
struct ComponentHandler {};
template<typename T>
using ComponentIdGenerator = utils::UniqueIdPerBaseGenerator<T, ComponentTag>;

class Entity {
public:

    template<typename Component, typename ...Args>
    void addComponent(Args... args) {
        _componentsMask.set(ComponentIdGenerator<Component>::GetGeneratedId());
        std::cout << typeid(Component).name() << " " << ComponentIdGenerator<Component>::GetGeneratedId() << std::endl;
    }

    template<typename Component>
    void removeComponent() {};

    template<typename Component>
    ComponentHandler<Component> getComponent() {
        auto id = ComponentIdGenerator<Component>::GetGeneratedId();
        assert((_componentsMask[id] == 1) && "No component ");
        return ComponentHandler<Component>{};
    }

    template<typename Component>
    ComponentHandler<Component> hasComponent() {
        auto id = ComponentIdGenerator<Component>::GetGeneratedId();
        return ComponentHandler<Component>{};
    }

    void destroy() {};

private:
    Entity() = default;
    Entity(Id id) : _id(id) {}

    std::bitset<MAX_COMPONENTS> _componentsMask;
    Id _id;

    friend class EntityManager;
};
}