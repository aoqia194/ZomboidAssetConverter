#include "pzw.hpp"

namespace pz::pzw {
    [[maybe_unused]]
    pugi::xml_node objectgroup_node(pugi::xml_node &parent,
                                    const std::string &name,
                                    const std::string &colour,
                                    const std::string &default_value) {
        auto obj = parent.append_child("objectgroup");
        obj.append_attribute("name") = name;
        obj.append_attribute("color") = colour;
        obj.append_attribute("defaulttype") = default_value;
        return obj;
    }

    [[maybe_unused]]
    pugi::xml_node objectgroup_node(pugi::xml_node &parent,
                                    const std::string &name,
                                    const std::string &colour) {
        auto obj = parent.append_child("objectgroup");
        obj.append_attribute("name") = name;
        obj.append_attribute("color") = colour;
        obj.append_attribute("defaulttype") = name;
        return obj;
    }

    [[maybe_unused]]
    pugi::xml_node objecttype_node(pugi::xml_node &parent, const std::string &name) {
        auto obj = parent.append_child("objecttype");
        obj.append_attribute("name") = name;
        return obj;
    }

    [[maybe_unused]]
    pugi::xml_node propertydef_node(pugi::xml_node &parent,
                                    const std::string &name,
                                    const std::string &default_value,
                                    const std::string &enum_value) {
        auto obj = parent.append_child("propertydef");
        obj.append_attribute("name") = name;
        obj.append_attribute("default") = default_value;
        if (enum_value == "!") obj.append_attribute("enum") = name;
        else if (!enum_value.empty()) obj.append_attribute("enum") = enum_value;
        return obj;
    }

    [[maybe_unused]]
    pugi::xml_node propertydef_node(pugi::xml_node &parent,
                                    const std::string &name,
                                    const std::string &default_value) {
        auto obj = parent.append_child("propertydef");
        obj.append_attribute("name") = name;
        obj.append_attribute("default") = default_value;
        return obj;
    }

    [[maybe_unused]]
    pugi::xml_node propertyenum_node(pugi::xml_node &parent,
                                     const std::string &name,
                                     const std::string &choices,
                                     const bool multi = false) {
        auto obj = parent.append_child("propertyenum");
        obj.append_attribute("name") = name;
        obj.append_attribute("choices") = choices;
        obj.append_attribute("multi") = multi;
        return obj;
    }


    void add_objectgroups(pugi::xml_node &parent) {
        objectgroup_node(parent, "ParkingStall", "#ff007f");
        objectgroup_node(parent, "TownZone", "#aa0000");
        objectgroup_node(parent, "Forest", "#00aa00");
        objectgroup_node(parent, "Nav", "#55aaff");
        objectgroup_node(parent, "DeepForest", "#003500");
        objectgroup_node(parent, "Vegitation", "#b3b300");
        objectgroup_node(parent, "TrailerPark", "#f50000");
        objectgroup_node(parent, "Farm", "#55ff7f");
        objectgroup_node(parent, "FarmLand", "#bcff7d");
        objectgroup_node(parent, "WaterFlow", "#0000ff");
        objectgroup_node(parent, "WaterZone", "#0000ff");
        objectgroup_node(parent, "Mannequin", "#0000ff");
        objectgroup_node(parent, "RoomTone", "#0000ff");
    }


    void add_objecttypes(pugi::xml_node &parent) {
        objecttype_node(parent, "TownZone");
        objecttype_node(parent, "Forest");
        objecttype_node(parent, "DeepForest");
        objecttype_node(parent, "Nav");
        objecttype_node(parent, "Vegitation");
        objecttype_node(parent, "TrailerPark");
        objecttype_node(parent, "Farm");
        objecttype_node(parent, "ParkingStall");
        objecttype_node(parent, "FarmLand");
        objecttype_node(parent, "WaterFlow");
        objecttype_node(parent, "WaterZone");
        objecttype_node(parent, "Mannequin");
        objecttype_node(parent, "RoomTone");
        objecttype_node(parent, "SpawnPoint");
    }


    void add_templates(pugi::xml_node &parent) {
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
        auto waterflows = parent.append_child("template");
        waterflows.append_attribute("name") = "WaterFlowS";
        auto waterflows_property = waterflows.append_child("property");
        waterflows_property.append_attribute("name") = "WaterDirection";
        waterflows_property.append_attribute("value") = 180;
        auto wfs_speed = wfn.append_child("property");
        wfs_speed.append_attribute("name") = "WaterSpeed";
        wfs_speed.append_attribute("value") = "1.0";
        // WaterFlowE
        auto waterflowe = parent.append_child("template");
        waterflowe.append_attribute("name") = "WaterFlowE";
        auto waterflowe_property = waterflowe.append_child("property");
        waterflowe_property.append_attribute("name") = "WaterDirection";
        waterflowe_property.append_attribute("value") = 90;
        auto wfe_speed = wfn.append_child("property");
        wfe_speed.append_attribute("name") = "WaterSpeed";
        wfe_speed.append_attribute("value") = "1.0";
        // WaterFlowW
        auto waterfloww = parent.append_child("template");
        waterfloww.append_attribute("name") = "WaterFlowW";
        auto waterfloww_property = waterfloww.append_child("property");
        waterfloww_property.append_attribute("name") = "WaterDirection";
        waterfloww_property.append_attribute("value") = 270;
        auto wfw_speed = wfn.append_child("property");
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
        sp.append_child("description").text().set(
            "This template holds the default set of properties for all spawn points in the world.");
        auto sp_professions = sp.append_child("property");
        sp_professions.append_attribute("name") = "Professions";
        sp_professions.append_attribute("value") = "all";
    }


    void add_propertydefs(pugi::xml_node &parent) {
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

    void add_propertyenums(pugi::xml_node &parent) {
        propertyenum_node(parent, "Direction", "N,S,W,E");
        propertyenum_node(parent, "Pose", "pose01,pose02,pose03");
        propertyenum_node(parent, "Skin", "White,Black");
        propertyenum_node(parent, "RoomTone", "Generic,Barn,Mall,Warehouse,Prison,Church,Office,"
                          "Factory");
        propertyenum_node(parent, "Professions", "all,unemployed,policeofficer,constructionworker,"
                          "securityguard,parkranger,fireofficer",
                          true);
    }
}
