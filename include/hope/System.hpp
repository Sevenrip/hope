#pragma once

#include "Config.hpp"
#include "EntityManager.hpp"

namespace hope {

class BaseSystem {
public:
    virtual ~BaseSystem() = default;

    virtual void update(EntityManager &entities, TimeDt dt) = 0;
    virtual void configure() {}
protected:
    static Id s_systemIdGenerator;         
};

template <typename T>
class System : BaseSystem {
private:
    static Id GetSystemId() {
        static Id systemId = ++s_systemIdGenerator;
        return systemId;
    }
    friend class SystemManager;

};

}