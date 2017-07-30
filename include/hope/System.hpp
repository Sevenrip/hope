#pragma once

#include "Config.hpp"
#include "EntityManager.hpp"

namespace hope {

class System  {
public:
    virtual void update(EntityManager &em, TimeDt dt) = 0;
    virtual void configure() {}
    
    friend class SystemManager;

};

}