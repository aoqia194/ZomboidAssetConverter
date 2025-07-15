#ifndef LOTPACK_HPP
#define LOTPACK_HPP

#include <cstdint>
#include <list>
#include <vector>

#include "lotheader.hpp"

#include "../../main.hpp"

namespace map::types
{
    //! TODO: Document.
    class lotpack
    {
        //! X position of the lot in the world.
        uint32_t wx{};

        //! Y position of the lot in the world.
        uint32_t wy{};

        //! The linked header for this lot.
        lotheader *header{};

    public:
        lotpack(uint32_t wx, uint32_t wy, lotheader *header);

        //! Some offset in the pack file.
        int64_t offset{};

        //! TODO: Document.
        std::array<std::array<std::vector<uint32_t>, CHUNKGRID_SIZE>, CHUNKGRID_SIZE>
        room_ids{};

        //! A flat vector matrix storing arbitrary XYZ data.
        //! Still don't entirely know what this is.
        std::array<std::array<std::vector<std::list<int32_t> >, CHUNKGRID_SIZE>,
                   CHUNKGRID_SIZE> data{};

        //! NOTE: I hope data doesn't go negative....
        std::vector<std::vector<uint32_t> > offset_in_data{};
    };
}

#endif //LOTPACK_HPP
