#include "lotpack.hpp"
#include "..\main.hpp"

namespace pz {
    lotpack::lotpack(const uint32_t wx, const uint32_t wy, lotheader *header) {
        this->wx = wx;
        this->wy = wy;
        this->header = header;

        // TODO: Is CHUNKGRID_WIDTH really the right maxsz to use here? Can't it go higher??
        this->data.resize(CHUNKGRID_SIZE);
        for (int x = 0; x < CHUNKGRID_SIZE; x++) {
            this->data[x].resize(CHUNKGRID_SIZE);
            for (int y = 0; y < CHUNKGRID_SIZE; y++) {
                this->data[x][y].resize(this->header->levels);
            }
        }

        this->room_ids.resize(CHUNKGRID_SIZE);
        for (int x = 0; x < CHUNKGRID_SIZE; x++) {
            this->room_ids[x].resize(CHUNKGRID_SIZE);
            for (int y = 0; y < CHUNKGRID_SIZE; y++) {
                this->room_ids[x][y].resize(this->header->levels);
            }
        }
    }
}
