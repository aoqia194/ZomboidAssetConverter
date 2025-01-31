#ifndef MAP_HPP
#define MAP_HPP

#include "main.hpp"

#include "map\lotheader.hpp"
#include "map\lotpack.hpp"

class Map {
public:
private:
    uint32_t ;
};

namespace map {
    //! The index of the currently processed header/pack file.
    //! Used to access the _lotheaders and _lotpacks.
    inline uint32_t _index;
    //! The currently processed header/pack file chunk's X position in the world.
    inline uint32_t _worldx;
    //! The currently processed header/pack file chunk's Y position in the world.
    inline uint32_t _worldy;
    //! The width of the entire map.
    inline uint32_t _width;
    //! The height of the entire map.
    inline uint32_t _height;

    inline std::deque<lotheader> _lotheaders;
    inline std::deque<lotpack> _lotpacks;

    //! Reconstructs a PZW file for the map and exports it.
    //! @param out The pzw file.
    //! @return True if the PZW file was exported successfully.
    bool write_pzw(const fs::path &out);

    //! Loads the lotpack into a struct.
    //! @param in The lotpack file.
    //! @return True if the lotpack was parsed successfully.
    bool read_lotpack(const fs::path &in);

    //! Loads the lotheader into a struct.
    //! @param in The header file.
    //! @return True if the lotheader was parsed successfully.
    bool read_lotheader(const fs::path &in);
}

#endif //MAP_HPP
