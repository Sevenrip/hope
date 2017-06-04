#pragma once

#include <vector>

#include "Entity.hpp"

namespace hope {

class EntityManager {
public:
    Entity creatEntity();

    template<typename T>
    Entity createEntityWith();

private:
    std::vector<Entity> _entities;
};
}