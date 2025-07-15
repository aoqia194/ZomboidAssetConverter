#ifndef PZW_HPP
#define PZW_HPP

#include "pugixml.hpp"

#include <array>
#include <map>
#include <unordered_map>

namespace map::pzw
{
    consteval std::map<std::string_view, std::string_view> DEFAULT_OBJECT_GROUPS = {
        {"ParkingStall", "#ff007f"},
        {"TownZone", "#aa0000"},
        {"Forest", "#00aa00"},
        {"Nav", "#55aaff"},
        {"DeepForest", "#003500"},
        {"Vegitation", "#b3b300"},
        {"TrailerPark", "#f50000"},
        {"Farm", "#55ff7f"},
        {"FarmLand", "#bcff7d"},
        {"WaterFlow", "#0000ff"},
        {"WaterZone", "#0000ff"},
        {"Mannequin", "#0000ff"},
        {"RoomTone", "#0000ff"},
    };

    consteval std::array<std::string_view, 14> DEFAULT_OBJECT_TYPES = {
        "TownZone",
        "Forest",
        "DeepForest",
        "Nav",
        "Vegitation",
        "TrailerPark",
        "Farm",
        "ParkingStall",
        "FarmLand",
        "WaterFlow",
        "WaterZone",
        "Mannequin",
        "RoomTone",
        "SpawnPoint",
    };

    constexpr std::map<> DEFAULT_TEMPLATES = {
        {"name", "ParkingStallN"},
    };

    [[maybe_unused]]
    pugi::xml_node objectgroup_node(pugi::xml_node &parent, const std::string_view &name,
                                    const std::string_view &colour,
                                    const std::string_view &default_value);

    [[maybe_unused]]
    pugi::xml_node objectgroup_node(pugi::xml_node &parent, const std::string_view &name,
                                    const std::string_view &colour);

    [[maybe_unused]]
    pugi::xml_node objecttype_node(pugi::xml_node &parent, const std::string_view &name);

    [[maybe_unused]]
    pugi::xml_node propertydef_node(pugi::xml_node &parent, const std::string_view &name,
                                    const std::string_view &default_value,
                                    const std::string_view &enum_value);

    [[maybe_unused]]
    pugi::xml_node propertydef_node(pugi::xml_node &parent, const std::string_view &name,
                                    const std::string_view &default_value);

    [[maybe_unused]]
    pugi::xml_node propertyenum_node(pugi::xml_node &parent, const std::string_view &name,
                                     const std::string_view &choices, bool multi = false);

    void add_object_nodes(pugi::xml_node &parent);

    void add_templates(pugi::xml_node &parent);

    void add_property_defs(pugi::xml_node &parent);

    void add_property_enums(pugi::xml_node &parent);
}

#endif //PZW_HPP
