#include "lotpack.hpp"

#include "../../main.hpp"

namespace pz::pzw
{
    lotpack::lotpack(const uint32_t wx, const uint32_t wy, lotheader *header) : wx(wx),
        wy(wy),
        header(header)
    {
        // TODO: Is CHUNKGRID_SIZE really the right maxsz to use here?
        //       Can't it go higher??

        // Disabled and merged room_ids for loop here since the two parent arrays have
        //     the same size. If these aren't the same, stuff is going to break though.
        for (size_t x = 0u; x < this->data.size(); x++) {
            for (size_t y = 0u; y < this->data.size(); y++) {
                this->data[x][y].resize(this->header->levels);
                this->room_ids[x][y].resize(this->header->levels);
            }
        }
    }
}
