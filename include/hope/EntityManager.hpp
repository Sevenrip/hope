#pragma once

#include <vector>
#include <bitset>

#include "utils/IdGenerator.hpp"
#include "memory/MemoryPool.hpp"


namespace hope {

class EntityManager;

struct ComponentTag {};

template<typename T>
using ComponentIdGenerator = utils::UniqueIdPerBaseGenerator<T, ComponentTag>;

class Entity {
public:
    template<typename Component, typename ...Args>
    void addComponent(Args... args);

    template<typename Component>
    void removeComponent();

    template<typename Component>
    bool hasComponent() const;        

    /*template<typename Component>
    ComponentHandle<Component> component() {
        auto id = ComponentIdGenerator<Component>::AssignedId();
        return ComponentHandler<Component>(_manager, _manager.component<Component>(_id), _id);
    }*/

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


template<typename Component, typename ...Args>
void Entity::addComponent(Args... args) {
    _manager.createComponent<Component>(_id, std::forward<Args>(args)...);
}

template<typename Component>
void Entity::removeComponent() {
    _manager.removeComponent<Component>(_id);
};

/*template<typename Component>
ComponentHandle<Component> component() {
    auto id = ComponentIdGenerator<Component>::AssignedId();
    return ComponentHandler<Component>(_manager, _manager.component<Component>(_id), _id);
}*/

template<typename Component>
bool Entity::hasComponent() const {
    auto maskBit = ComponentIdGenerator<Component>::AssignedId();
    return  this->componentsMask().at(_id).test(maskBit);
}

std::vector<std::bitset<MAX_COMPONENTS>> Entity::componentsMask() const {
    return _manager.componentsMask();
}

}