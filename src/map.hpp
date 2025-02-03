#ifndef MAP_HPP
#define MAP_HPP

#include <vector>

#include "main.hpp"

#include "map\types\lotheader.hpp"
#include "map\types\lotpack.hpp"

namespace pz {
    class map {
        //! Name of the map.
        std::string name;

        //! Width of the map in chunks.
        uint32_t width = 0;

        //! Height of the map in chunks.
        uint32_t height = 0;

        //! The world starting position on the X axis.
        uint32_t worldx = 0;

        //! The world starting position on the Y axis;
        uint32_t worldy = 0;

        //! The map's lotheader files.
        std::vector<std::vector<lotheader> > lotheaders{};

        //! The map's lotpack files.
        std::vector<std::vector<lotpack> > lotpacks{};

        //! Writes the final PZW WorldEd project file to the out dir.
        //! @param out Directory to write the file to.
        //! @return True if the file was written successfully.
        bool write_pzw(const fs::path &out);

        //! Writes a TMX file constructed from lot chunks to the out dir.
        //! @param out Directory to write the file to.
        //! @return True if the file was written succefsfully.
        bool write_tmx(const fs::path &out);

        //! Writes a TBX file constructed from lot chunks to the out dir.
        //! @param out Directory to write the file to.
        //! @returns True if the file was written successfully.
        bool write_tbx(const fs::path &out);

        //! Reads the lot data from lotpack files.
        //! @param in Lotpack file to read.
        //! @param wx X position of the lotpack chunk in the world.
        //! @param wy Y position of the lotpack chunk in the world.
        //! @return True if the file was read successfully.
        bool read_lotpack(const fs::path &in, uint32_t wx, uint32_t wy);

        //! Reads the data from lotheader files.
        //! @param in Lotheader file to read.
        //! @param wx X position of the lotheader chunk in the world.
        //! @param wy Y position of the lotheader chunk in the world.
        //! @return True if the file was read successfully.
        bool read_lotheader(const fs::path &in, uint32_t wx, uint32_t wy);

    public:
        //! Writes the reconstructed map into original mapping files like PZW, TMX, TBX.
        //! @param out Directory to write the map to.
        //! @return True if the map was written to the filesystem successfully.
        bool write(const fs::path &out);

        //! Reconstructs the map into manipulatable data. Iterates over `in` twice.
        //! @param in Directory to read the map from.
        //! @return True if the map was read into memory successfully.
        bool read(const fs::path &in);
    };
}

#endif //MAP_HPP
