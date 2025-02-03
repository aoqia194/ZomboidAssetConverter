#ifndef BUILDINGDEF_HPP
#define BUILDINGDEF_HPP

#include <cstdint>
#include <vector>

#include "roomdef.hpp"

namespace pz {
    class buildingdef {
        buildingdef();

        uint32_t width;

        uint32_t height;

    public:
        uint32_t room_count;

        std::vector<roomdef> rooms{};
    };
}

#endif //BUILDINGDEF_HPP
