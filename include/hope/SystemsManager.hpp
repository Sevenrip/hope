#pragma once

#include <vector>
#include <memory>

#include "System.hpp"
#include "detail/IdGenerator.hpp"

namespace hope {

struct SystemTag {};

template<typename T>
using SystemIdGenerator = detail::UniqueIdPerBaseGenerator<T, SystemTag>;

class SystemsManager {
public:
    SystemsManager() = default;

    EntityManager & entities();
    
    void updateAll(double dt);

    template<typename T, typename ...Args>
    void addSystem(Args&& ...args) {
        _systems.emplace_back(std::forward<Args>(args)...);
    }
    
    template <typename T>
    void update(double dt) {
        auto id = SystemIdGenerator<T>::AssignedId();
        _systems[id]->update(_entityManager, dt);
    }

private:
    EntityManager _entityManager;
    std::vector<std::shared_ptr<System>> _systems;

};

}