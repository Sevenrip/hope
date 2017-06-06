#pragma once

#include "../Config.hpp"

namespace hope {
namespace utils {
    
template<typename Tag>
struct UniqueIdHolder{
    static Id s_Id;
};


template<typename Tag>
Id UniqueIdHolder<Tag>::s_Id = 0;

template<typename T, typename Base>
struct UniqueIdPerBaseGenerator : UniqueIdHolder<Base> {
    static Id AssignedId() {
        static Id id = UniqueIdHolder<Base>::s_Id++;
        return id;
    }
};

template<typename Tag>
struct IncrementalIdGenerator {
    static Id GenerateId() {
        static Id id = 0;
        return id++;
    }
};


}
}