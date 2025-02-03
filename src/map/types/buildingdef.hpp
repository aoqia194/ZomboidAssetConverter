#ifndef BUILDINGDEF_HPP
#define BUILDINGDEF_HPP

#include <cstdint>
#include <vector>

#include "roomdef.hpp"

namespace pz {
    //! TODO: Document
    struct buildingdef {
        uint32_t room_count;

        std::vector<roomdef> rooms{};
    };
}

#endif //BUILDINGDEF_HPP
