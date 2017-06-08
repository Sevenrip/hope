#include "hope/EntityManager.hpp"

namespace hope {
std::vector<ComponentsMask> Entity::componentsMask() const {
    return _manager.componentsMask();
} 

}