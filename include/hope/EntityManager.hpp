#pragma once

#include <vector>
#include <bitset>
#include <functional>
#include <type_traits>

#include <iostream>

#include "detail/IdGenerator.hpp"
#include "detail/MemoryPool.hpp"
#include "detail/MetaUtils.hpp"


namespace hope {

class EntityManager;
class Entity;

template<typename Component>
class ComponentHandle {
public:
    ComponentHandle(EntityManager & manager, const Entity * entity, Component * component)  
    : _manager(manager), _component(component), _entity(entity) {}

    Component * operator->() const { return _component;}

private:
        EntityManager & _manager;
        Component * _component = nullptr;
        const Entity * _entity = nullptr;
};

struct ComponentTag {};

template<typename T>
using ComponentIdGenerator = detail::UniqueIdPerBaseGenerator<T, ComponentTag>;

class Entity {
public:
    template<typename Component, typename ...Args>
    void addComponent(Args... args);

    template<typename Component>
    void removeComponent();

    template<typename Component>
    bool hasComponent() const;        

    template<typename Component>
    ComponentHandle<Component> component() const;
    std::vector<ComponentsMask> componentsMask() const;     

    Id id() const;

    void destroy();

private:
    Entity(EntityManager & manager, Id id) : _manager(manager), _id(id) {}
    Entity() = default;

    EntityManager & _manager;
    Id _id;

    friend class EntityManager;
};

namespace detail {

struct ComponentHelper {
    virtual void removeComponent(Entity & entity) = 0;
};

template<typename Component>
struct TypedComponentHelper : ComponentHelper {
    virtual void removeComponent(Entity & entity) final {
        entity.removeComponent<Component>();
    }
}; 
}   

class EntityManager : private detail::IncrementalIdGenerator<EntityManager> {
public:
    EntityManager()  { _componentsMem.reserve(MAX_COMPONENTS); }

    Entity createEntity() {
        const std::pair<Id,bool> result = [this]() -> std::pair<Id,bool> {
            if(_recycledEntities.empty()) {
                return {GenerateId(), true};
            }
            else {
                Id id = _recycledEntities.back();
                _recycledEntities.pop_back();
                return {id, false};
            }
        }();
     
        if(result.second) {  //if new id
             _entities.push_back(true);
             _componentsMask.emplace_back();
        }
        else {
            _entities[result.first] = true;
        }
       
        Entity entity(const_cast<EntityManager&>(*this),result.first);
        return entity;
    }

    template<typename ...Components>
    Entity createEntityWithComponents(Components&& ...components) {
        Entity entity = createEntity();
        (void) (int[]){ (addComponent<Components>(entity.id(), std::forward<Components>(components)),0)... };
        _entities.push_back(true);
        return entity;
    }

    void deleteEntity(Id id) {
        ComponentsMask mask = _componentsMask[id];
        _entities[id] = false;
        for(auto i = 0; i < _nrOfComponentTypesUsed; i++) {
            if(mask.test(i)) {
                Entity entity(const_cast<EntityManager&>(*this),id);
                _componentsHelpers[i]->removeComponent(entity);
            }
        }
        _recycledEntities.push_back(id);
    }

    template<typename ...Components>
    void forEach(typename detail::identity<const std::function<void(Components&...)> &>::type f) {
        for(size_t id = 0; id < _entities.size(); id++) {
            if(!_entities[id]) {
                continue;
            }

            auto mask = _componentsMask[id];
            if(mask != maskFromComponents<Components...>()) {
                continue;
            }
  
            detail::callFunctionWithTupleAsArgs(f, makeComponentsTuple<Components...>(id));     
        }
    }

   const std::vector<bool>& entities() const { return _entities; }

private:
    template<typename ...Components>
    ComponentsMask maskFromComponents() {
        ComponentsMask mask;
        (void) (int[]){ (mask.set(ComponentIdGenerator<Components>::AssignedId()),0)... };
        return mask;
    }

    template<typename ...Components>
    decltype(auto) makeComponentsTuple(Id id) {
        return std::make_tuple(componentRef<Components>(id)...);
    }

    template<typename Component>
    Component * component(Id id) {
        using DecayedComponentType = std::decay_t<Component>;
        auto componentId = ComponentIdGenerator<DecayedComponentType>::AssignedId();
        if(!_componentsMask[id].test(componentId)) {
            return nullptr;
        }
        return static_cast<DecayedComponentType*>(_componentsMem[id]->getElementMem(id));
    }

    template<typename Component>
    std::decay_t<Component>& componentRef(Id id) {
        return *component<Component>(id); 
    }

    template<typename Component, typename ...Args>
    void addComponent(Id id, Args ...args) {
        auto componentId = ComponentIdGenerator<Component>::AssignedId();

        if(_nrOfComponentTypesUsed >= componentId) { //new Component Type being added
            _nrOfComponentTypesUsed++;
            _componentsHelpers.push_back(std::make_unique<detail::TypedComponentHelper<Component>>());
            _componentsMem.push_back(std::make_unique<detail::MemoryPool<Component>>()); 
        }
        
        new (_componentsMem[id]->getElementMem(id)) Component(std::forward<Args>(args)...);
        _componentsMask[id].set(componentId);
    }

    template<typename Component>
    void removeComponent(Id id) {
        auto componentId = ComponentIdGenerator<Component>::AssignedId();
            if(!_componentsMask[id].test(componentId)) {
            return;
        }        
        _componentsMask[id].reset(componentId);
        _componentsMem[id]->deleteElement(id);
    }

    std::vector<ComponentsMask> componentsMask() const { return _componentsMask;}

    std::vector<bool> _entities;
    std::vector<Id> _recycledEntities;
    std::vector<std::unique_ptr<detail::ComponentHelper>> _componentsHelpers; 
    std::vector<ComponentsMask> _componentsMask;
    std::vector<std::unique_ptr<detail::BaseMemoryPool>> _componentsMem;

    Id _nrOfComponentTypesUsed = 0;

    friend class Entity;
};


//Entity Implementation
template<typename Component, typename ...Args>
void Entity::addComponent(Args... args) {
    _manager.addComponent<Component>(_id, std::forward<Args>(args)...);
}

template<typename Component>
void Entity::removeComponent() {
    _manager.removeComponent<Component>(_id);
};

template<typename Component>
ComponentHandle<Component> Entity::component() const {
    return ComponentHandle<Component>(_manager, this, _manager.component<Component>(_id));
}

template<typename Component>
bool Entity::hasComponent() const {
    auto maskBit = ComponentIdGenerator<Component>::AssignedId();
    return  this->componentsMask().at(_id).test(maskBit);
}

}