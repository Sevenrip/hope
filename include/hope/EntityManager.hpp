#pragma once

#include <vector>

#include "utils/IdGenerator.hpp"
#include "memory/MemoryPool.hpp"
#include "Entity.hpp"

namespace hope {

class EntityManager : private utils::IncrementalIdGenerator<EntityManager> {
public:
    EntityManager() : _components(MAX_COMPONENTS) {}

    Entity createEntity() {
        auto id = GenerateId();
        Entity entity(const_cast<EntityManager&>(*this),id);
        _entities.push_back(id);
        return entity;
    }

    template<typename ...Components>
    Entity createEntityWithComponents(Components&& ...components) {
        Entity entity = createEntity();
        (void) (int[]){ (createComponent<Components>(entity.id(), std::forward<Components>(components)),0)... };
        _entities.push_back(entity.id());
        return entity;
    }

private:

    template<typename Component>
    Component * component(Id id) {
        auto componentId = ComponentIdGenerator<Component>::AssignedId();
        if(!_componentsMask[id].test(componentId)) {
            return nullptr;
        }
        return static_cast<Component*>(_components[id].getElementMem<Component>(id));
    }

    template<typename Component, typename ...Args>
    void createComponent(Id id, Args ...args) {
        auto componentId = ComponentIdGenerator<Component>::AssignedId();
         _componentsMask[id].set(componentId);
        new (_components[id].getElementMem<Component>(id)) Component(std::forward<Args>(args)...);
    }

    template<typename Component>
    void removeComponent(Id id) {
        auto componentId = ComponentIdGenerator<Component>::AssignedId();
            if(!_componentsMask[id].test(componentId)) {
            return;
        }        
        _componentsMask[id].set(componentId);
        _components[id].deleteElement<Component>(id);
    }

    std::vector<std::bitset<MAX_COMPONENTS>> componentsMask() const { return _componentsMask;}

    std::vector<Id> _entities;
    std::vector<std::bitset<MAX_COMPONENTS>> _componentsMask;
    std::vector<mem::MemoryPool> _components;

    friend class Entity;
};
}