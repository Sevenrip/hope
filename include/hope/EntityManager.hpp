#pragma once

#include <vector>

#include "Entity.hpp"
#include "utils/IdGenerator.hpp"

namespace hope {

class EntityManager : private utils::IncrementalIdGenerator<EntityManager> {
public:
    Entity createEntity() {
        auto id = GenerateId();
        Entity entity(id);
        _entities.push_back(entity);
        return entity;
    }

    template<typename ...T>
    Entity createEntityWithComponents(T&& ...t) {
        Entity entity = createEntity();
        (void) std::initializer_list<int> { (entity.addComponent<T>(),0)... };
        _entities.push_back(entity);
        return entity;
    }

private:
    std::vector<Entity> _entities;
};
}