#ifndef LOTHEADER_H
#define LOTHEADER_H

#include <string>
#include <vector>

#include "buildingdef.hpp"
#include "roomdef.hpp"

namespace pz {
    //! Used by Project Zomboid in exported maps via the .lotheader file.
    class lotheader {
        //! X position in the world.
        uint32_t wx;
        //! Y position in the world.
        uint32_t wy;

    public:
        lotheader(uint32_t wx, uint32_t wy);

        //! Lotheader version. Always 0 since <=41.78.16.
        uint32_t version;

        //! Number of tilesheets used by this lot.
        uint32_t tile_count;

        std::vector<std::string> tiles{};

        //! Chunk width in tiles.
        uint32_t width;

        //! Chunk height in tiles.
        uint32_t height;

        //! The maximum levels for this chunk.
        uint32_t levels;

        uint32_t room_count;

        std::vector<roomdef> rooms{};

        uint32_t building_count;

        std::vector<buildingdef> buildings{};

        //! Stores the zombie intensity data for chunks in a matrix.
        //! Is used to reconstruct the ZombieSpawns file.
        std::vector<uint32_t> intensity{};
    };
}

#endif //LOTHEADER_H
