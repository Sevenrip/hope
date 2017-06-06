#include "hope/Entity.hpp"
#include "hope/EntityManager.hpp"
#include <vector>
#include <bitset>

namespace hope {

std::vector<std::bitset<MAX_COMPONENTS>> Entity::componentsMask() const {
    return _manager.componentsMask();
}

}