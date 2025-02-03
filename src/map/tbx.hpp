#ifndef TBX_HPP
#define TBX_HPP

#include "pugixml.hpp"

#include "..\main.hpp"

namespace pz {
    class tbx {
        [[maybe_unused]]
        pugi::xml_node tile_node(pugi::xml_node &parent,
                                 const std::string &name,
                                 const std::string &colour,
                                 const std::string &default_value);

        [[maybe_unused]]
        pugi::xml_node tileentry_node(pugi::xml_node &parent,
                                      const std::string &name,
                                      const std::string &colour);

        void add_floor(pugi::xml_node &parent);

        void add_room(pugi::xml_node &parent);

        void add_furniture(pugi::xml_node &parent);

        void add_tileentry(pugi::xml_node &parent);

    public:
        bool write_tbx(const fs::path &out);
    };
}

#endif //TBX_HPP
