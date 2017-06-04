#pragma once

#include <vector>
#include <memory>

#include "EntityManager.hpp"
#include "System.hpp"
#include "Config.hpp"

namespace hope {

class BaseSystem;

class SystemManager {
public:
    SystemManager(EntityManager & entitySystem) : _entityManager(entitySystem) {}
    
    void updateAll(double dt);

    template<typename T, typename ...Args>
    void addSystem(Args&& ...args) {
        _systems.emplace_back(std::forward<Args>(args)...);
    }
    
    template <typename T>
    void update(double dt) {
        _systems[T::GetSystemId()]->update(_entityManager, dt);
    }
private:

    EntityManager & _entityManager;
    std::vector<std::shared_ptr<BaseSystem>> _systems;

};

}