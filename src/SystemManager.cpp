#include "hope/SystemManager.hpp"

namespace hope {

void SystemManager::updateAll(double dt) {
    for(auto system : _systems) {
        system->update(_entityManager, dt);
    }
}

}