#ifndef ROOMDEF_HPP
#define ROOMDEF_HPP

#include <string>

#include "metaobject.hpp"
#include "roomrect.hpp"

//! TODO: Document
struct roomdef {
    std::string name;
    uint32_t level;
    uint32_t roomrect_count;
    std::vector<roomrect> roomrects{};
    buildingdef building;
    uint32_t objects_count;
    std::vector<metaobject> objects{};
};

#endif //ROOMDEF_HPP
