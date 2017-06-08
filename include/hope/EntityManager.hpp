#pragma once

#include <vector>
#include <bitset>

#include "detail/IdGenerator.hpp"
#include "detail/MemoryPool.hpp"


namespace hope {

class EntityManager;
class Entity;

struct ComponentHandle {
    ComponentHandle(EntityManager & , Entity * ) {}//  : _manager(manager), _entity(entity) {}
    private:
       // EntityManager & _manager;
        //Entity * _entity = nullptr;
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

    /*template<typename Component>
    ComponentHandle<Component> component() {
        auto id = ComponentIdGenerator<Component>::AssignedId();
        return ComponentHandler<Component>(_manager, _manager.component<Component>(_id), _id);
    }*/

    std::vector<ComponentsMask> componentsMask() const;     
    

    Id id() const { return _id;}

    void destroy() {};

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
    EntityManager() : _componentsMem(MAX_COMPONENTS) {}

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
        //if new id
        if(result.second) {
             _entities.push_back(true);
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
        (void) (int[]){ (createComponent<Components>(entity.id(), std::forward<Components>(components)),0)... };
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

private:

    template<typename Component>
    Component * component(Id id) {
        auto componentId = ComponentIdGenerator<Component>::AssignedId();
        if(!_componentsMask[id].test(componentId)) {
            return nullptr;
        }
        return static_cast<Component*>(_componentsMem[id]->getElementMem(id));
    }

    template<typename Component, typename ...Args>
    void createComponent(Id id, Args ...args) {
        auto componentId = ComponentIdGenerator<Component>::AssignedId();

        if(_nrOfComponentTypesUsed >= componentId) { //new Component Type being added
            _nrOfComponentTypesUsed++;
            _componentsHelpers.push_back(new detail::TypedComponentHelper<Component>());
        }
         _componentsMask[id].set(componentId);
        new (_componentsMem[id]->getElementMem(id)) Component(std::forward<Args>(args)...);
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
    std::vector<detail::ComponentHelper*> _componentsHelpers; 
    std::vector<ComponentsMask> _componentsMask;
    std::vector<detail::BaseMemoryPool*> _componentsMem;

    Id _nrOfComponentTypesUsed = 0;

    friend class Entity;
};


//Entity Implementation
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



}