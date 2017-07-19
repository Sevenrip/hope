#include "hope/EntityManager.hpp"

namespace hope {

Id Entity::id() const { return _id;}

void Entity::destroy() {};

std::vector<ComponentsMask> Entity::componentsMask() const {
    return _manager.componentsMask();
} 

}