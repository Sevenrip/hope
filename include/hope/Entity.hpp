#pragma once

#include <cassert>
#include <bitset>
#include <vector>

#include "Config.hpp"
#include "utils/IdGenerator.hpp"



namespace hope {

class EntityManager;
struct ComponentTag {};

template<typename T>
using ComponentIdGenerator = utils::UniqueIdPerBaseGenerator<T, ComponentTag>;

class Entity {
public:
    template<typename Component, typename ...Args>
    void addComponent(Args... args) {
        //_manager.createComponent<Component>(_id, std::forward<Args>(args)...);
    }

    template<typename Component>
    void removeComponent() {
        //_manager.removeComponent<Component>(_id);
    };

    /*template<typename Component>
    ComponentHandle<Component> component() {
        auto id = ComponentIdGenerator<Component>::AssignedId();
        return ComponentHandler<Component>(_manager, _manager.component<Component>(_id), _id);
    }*/

    template<typename Component>
    bool hasComponent() {
        auto maskBit = ComponentIdGenerator<Component>::AssignedId();
        return  this->componentsMask().at(_id).test(maskBit);
    }

    std::vector<std::bitset<MAX_COMPONENTS>> componentsMask() const;     
    

    Id id() const { return _id;}

    void destroy() {};

private:
    Entity(EntityManager & manager, Id id) : _manager(manager), _id(id) {}
    Entity() = default;

    EntityManager & _manager;
    Id _id;

    friend class EntityManager;
};
}