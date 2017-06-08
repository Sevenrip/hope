#pragma once

#include <limits>
#include <bitset>

namespace hope {
    constexpr unsigned int MAX_COMPONENTS = 64;
    using ComponentsMask = std::bitset<MAX_COMPONENTS>;
    using Id = unsigned int;
    using TimeDt = double;
}