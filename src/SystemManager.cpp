#include "hope/SystemsManager.hpp"
#include "hope/EntityManager.hpp"

namespace hope {

void SystemsManager::updateAll(double dt) {
    for(auto system : _systems) {
        system->update(_entityManager, dt);
    }
}

EntityManager & SystemsManager::entities() { return _entityManager; }

}