#ifndef BUILDINGDEF_HPP
#define BUILDINGDEF_HPP

#include <cstdint>

#include "roomdef.hpp"

//! TODO: Document
struct buildingdef {
    uint32_t room_count;
    std::deque<roomdef> rooms;
};

#endif //BUILDINGDEF_HPP
