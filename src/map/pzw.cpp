#include "pzw.hpp"

namespace pz::pzw
{
    [[maybe_unused]]
    pugi::xml_node objectgroup_node(pugi::xml_node &parent, const std::string_view &name,
                                    const std::string_view &colour,
                                    const std::string_view &default_value)
    {
        auto obj = parent.append_child("objectgroup");
        obj.append_attribute("name") = name;
        obj.append_attribute("color") = colour;
        obj.append_attribute("defaulttype") = default_value;
        return obj;
    }

    [[maybe_unused]]
    pugi::xml_node objectgroup_node(pugi::xml_node &parent, const std::string_view &name,
                                    const std::string_view &colour)
    {
        auto obj = parent.append_child("objectgroup");
        obj.append_attribute("name") = name;
        obj.append_attribute("color") = colour;
        obj.append_attribute("defaulttype") = name;
        return obj;
    }

    [[maybe_unused]]
    pugi::xml_node objecttype_node(pugi::xml_node &parent, const std::string_view &name)
    {
        auto obj = parent.append_child("objecttype");
        obj.append_attribute("name") = name;
        return obj;
    }

    [[maybe_unused]]
    pugi::xml_node propertydef_node(pugi::xml_node &parent, const std::string_view &name,
                                    const std::string_view &default_value,
                                    const std::string_view &enum_value)
    {
        auto obj = parent.append_child("propertydef");
        obj.append_attribute("name") = name;
        obj.append_attribute("default") = default_value;
        if (enum_value == "!")
            obj.append_attribute("enum") = name;
        else if (!enum_value.empty())
            obj.append_attribute("enum") = enum_value;
        return obj;
    }

    [[maybe_unused]]
    pugi::xml_node propertydef_node(pugi::xml_node &parent, const std::string_view &name,
                                    const std::string_view &default_value)
    {
        auto obj = parent.append_child("propertydef");
        obj.append_attribute("name") = name;
        obj.append_attribute("default") = default_value;
        return obj;
    }

    [[maybe_unused]]
    pugi::xml_node propertyenum_node(pugi::xml_node &parent, const std::string_view &name,
                                     const std::string_view &choices, const bool multi)
    {
        auto obj = parent.append_child("propertyenum");
        obj.append_attribute("name") = name;
        obj.append_attribute("choices") = choices;
        obj.append_attribute("multi") = multi;
        return obj;
    }

    void add_object_nodes(pugi::xml_node &parent)
    {
        for (const auto &[name, colour] : DEFAULT_OBJECT_GROUPS) {
            objectgroup_node(parent, name, colour);
        }

        for (const auto &type : DEFAULT_OBJECT_TYPES) {
            objecttype_node(parent, type);
        }
    }

    void add_templates(pugi::xml_node &parent)
    {
        // TODO: Should probably rewrite this using objects and stuff.

        // ParkingStallN
        auto psn = parent.append_child("template");
        psn.append_attribute("name") = "ParkingStallN";
        auto psn_dir = psn.append_child("property");
        psn_dir.append_attribute("name") = "Direction";
        psn_dir.append_attribute("value") = "N";
        // ParkingStallS
        auto pss = parent.append_child("template");
        pss.append_attribute("name") = "ParkingStallS";
        auto pss_dir = pss.append_child("property");
        pss_dir.append_attribute("name") = "Direction";
        pss_dir.append_attribute("value") = "S";
        // ParkingStallW
        auto psw = parent.append_child("template");
        psw.append_attribute("name") = "ParkingStallW";
        auto psw_dir = psw.append_child("property");
        psw_dir.append_attribute("name") = "Direction";
        psw_dir.append_attribute("value") = "W";
        // ParkingStallE
        auto pse = parent.append_child("template");
        pse.append_attribute("name") = "ParkingStallE";
        auto pse_dir = pse.append_child("property");
        pse_dir.append_attribute("name") = "Direction";
        pse_dir.append_attribute("value") = "E";
        // WaterFlowN
        auto wfn = parent.append_child("template");
        wfn.append_attribute("name") = "WaterFlowN";
        auto wfn_dir = wfn.append_child("property");
        wfn_dir.append_attribute("name") = "WaterDirection";
        wfn_dir.append_attribute("value") = 0;
        auto wfn_speed = wfn.append_child("property");
        wfn_speed.append_attribute("name") = "WaterSpeed";
        wfn_speed.append_attribute("value") = "1.0";
        // WaterFlowS
        auto wfs = parent.append_child("template");
        wfs.append_attribute("name") = "WaterFlowS";
        auto wfs_dir = wfs.append_child("property");
        wfs_dir.append_attribute("name") = "WaterDirection";
        wfs_dir.append_attribute("value") = 180;
        auto wfs_speed = wfs.append_child("property");
        wfs_speed.append_attribute("name") = "WaterSpeed";
        wfs_speed.append_attribute("value") = "1.0";
        // WaterFlowE
        auto wfe = parent.append_child("template");
        wfe.append_attribute("name") = "WaterFlowE";
        auto wfe_dir = wfe.append_child("property");
        wfe_dir.append_attribute("name") = "WaterDirection";
        wfe_dir.append_attribute("value") = 90;
        auto wfe_speed = wfe.append_child("property");
        wfe_speed.append_attribute("name") = "WaterSpeed";
        wfe_speed.append_attribute("value") = "1.0";
        // WaterFlowW
        auto wfw = parent.append_child("template");
        wfw.append_attribute("name") = "WaterFlowW";
        auto wfw_dir = wfw.append_child("property");
        wfw_dir.append_attribute("name") = "WaterDirection";
        wfw_dir.append_attribute("value") = 270;
        auto wfw_speed = wfw.append_child("property");
        wfw_speed.append_attribute("name") = "WaterSpeed";
        wfw_speed.append_attribute("value") = "1.0";
        // WaterZone
        auto wz = parent.append_child("template");
        wz.append_attribute("name") = "ParkingStallW";
        auto wz_ground = wz.append_child("property");
        wz_ground.append_attribute("name") = "WaterGround";
        wz_ground.append_attribute("value") = false;
        auto wz_shore = wz.append_child("property");
        wz_shore.append_attribute("name") = "WaterShore";
        wz_shore.append_attribute("value") = true;
        // RoomTone
        auto rt = parent.append_child("template");
        rt.append_attribute("name") = "ParkingStallW";
        auto rt_tone = rt.append_child("property");
        rt_tone.append_attribute("name") = "RoomTone";
        rt_tone.append_attribute("value") = "Generic";
        auto rt_entirebuilding = rt.append_child("property");
        rt_entirebuilding.append_attribute("name") = "EntireBuilding";
        rt_entirebuilding.append_attribute("value") = false;
        // SpawnPoint
        auto sp = parent.append_child("template");
        sp.append_attribute("name") = "ParkingStallW";
        sp.append_child("description").text().set("This template holds the default set of"
            " properties for all spawn points in the world.");
        auto sp_professions = sp.append_child("property");
        sp_professions.append_attribute("name") = "Professions";
        sp_professions.append_attribute("value") = "all";
    }

    void add_property_defs(pugi::xml_node &parent)
    {
        propertydef_node(parent, "Direction", "N", "!");
        propertydef_node(parent, "FaceDirection", "true");
        propertydef_node(parent, "WaterDirection", "0.0");
        propertydef_node(parent, "WaterSpeed", "0.0");
        propertydef_node(parent, "WaterGround", "false");
        propertydef_node(parent, "WaterShore", "true");
        propertydef_node(parent, "Female", "true");
        propertydef_node(parent, "Outfit", "");
        propertydef_node(parent, "Pose", "pose01", "!");
        propertydef_node(parent, "Script", "");
        propertydef_node(parent, "Skin", "White", "!");
        propertydef_node(parent, "RoomTone", "Generic", "!");
        propertydef_node(parent, "EntireBuilding", "false");
        propertydef_node(parent, "Professions", "all", "!")
            .append_child("description").text().set(
                "Comma-separated list of professions that may spawn here.  "
                "Use &quot;all&quot; to allow any profession to spawn here.");
    }

    void add_property_enums(pugi::xml_node &parent)
    {
        propertyenum_node(parent, "Direction", "N,S,W,E");
        propertyenum_node(parent, "Pose", "pose01,pose02,pose03");
        propertyenum_node(parent, "Skin", "White,Black");
        propertyenum_node(parent, "RoomTone",
                          "Generic,Barn,Mall,Warehouse,Prison,Church,Office,Factory");
        propertyenum_node(parent, "Professions",
                          "all,unemployed,policeofficer,constructionworker,"
                          "securityguard,parkranger,fireofficer", true);
    }
}