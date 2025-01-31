#ifndef PZW_HPP
#define PZW_HPP

#include "pugixml.hpp"

namespace pzw {
    [[maybe_unused]]
    pugi::xml_node objectgroup_node(pugi::xml_node &parent,
                                    const std::string &name,
                                    const std::string &colour,
                                    const std::string &default_value = name);

    [[maybe_unused]]
    pugi::xml_node objecttype_node(pugi::xml_node &parent, const std::string &name);

    [[maybe_unused]]
    pugi::xml_node propertydef_node(pugi::xml_node &parent,
                                    const std::string &name,
                                    const std::string &default_value,
                                    const std::string &enum_value = "");

    [[maybe_unused]]
    pugi::xml_node propertyenum_node(pugi::xml_node &parent,
                                     const std::string &name,
                                     const std::string &choices,
                                     bool multi = false);

    void add_objectgroups(pugi::xml_node &parent);

    void add_objecttypes(pugi::xml_node &parent);

    void add_templates(pugi::xml_node &parent);

    void add_propertydefs(pugi::xml_node &parent);

    void add_propertyenums(pugi::xml_node &parent);
}

#endif //PZW_HPP
