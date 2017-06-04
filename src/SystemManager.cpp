#include "hope/SystemsManager.hpp"

namespace hope {

void SystemsManager::updateAll(double dt) {
    for(auto system : _systems) {
        system->update(_entityManager, dt);
    }
}

}