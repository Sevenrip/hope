#pragma once

#include <vector>
#include <memory>

#include "EntityManager.hpp"
#include "System.hpp"
#include "Config.hpp"
#include "utils/IdGenerator.hpp"

namespace hope {

struct SystemTag {};

template<typename T>
using SystemIdGenerator = utils::UniqueIdPerBaseGenerator<T, SystemTag>;

class SystemsManager {
public:
    SystemsManager() = default;

    EntityManager & entities() { return _entityManager; }
    
    void updateAll(double dt);

    template<typename T, typename ...Args>
    void addSystem(Args&& ...args) {
        _systems.emplace_back(std::forward<Args>(args)...);
    }
    
    template <typename T>
    void update(double dt) {
        auto id = SystemIdGenerator<T>::GetGeneratedId();
        _systems[id]->update(_entityManager, dt);
    }

private:
    EntityManager _entityManager;
    std::vector<std::shared_ptr<System>> _systems;

};

}