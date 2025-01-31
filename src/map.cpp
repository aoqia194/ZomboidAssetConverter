#include "map.hpp"

#include <fstream>

#include "pugixml.hpp"

#include "map\pzw.hpp"

#include "spdlog\spdlog.h"

#define STREAM_READ(stream, out) stream.read(reinterpret_cast<char *>(&out), sizeof(out))
#define STREAM_READSZ(stream, out, size) stream.read(reinterpret_cast<char *>(&out), size)

namespace map {
    bool write_pzw(const fs::path &out) {
        create_directories(out.parent_path());

        pugi::xml_document pzw;

        auto world = pzw.append_child("world");
        world.append_attribute("version") = "1.0";
        world.append_attribute("width") = _width;
        world.append_attribute("height") = _height;

        // TODO: For bmp in bmps, create bmp node here.

        pzw::add_propertyenums(world);
        pzw::add_propertydefs(world);
        pzw::add_templates(world);
        pzw::add_objecttypes(world);
        pzw::add_objectgroups(world);

        return pzw.save_file(out.c_str(), "\t", pugi::format_default, pugi::encoding_utf8);
    }


    bool read_lotpack(const fs::path &in) {
        std::ifstream stream(in, std::ios::binary);
        if (!stream.is_open() || !stream.good()) {
            spdlog::error("Failed to open file {}", in.string());
            return false;
        }

        auto pack = lotpack();
        const auto &header = _lotheaders[_index];

        pack.data.resize(CHUNKGRID_WIDTH);
        for (uint32_t x = 0; x < CHUNKGRID_WIDTH; x++) {
            pack.data[x].resize(CHUNKGRID_WIDTH);
            for (uint32_t y = 0; y < CHUNKGRID_WIDTH; y++) {
                pack.data[x][y].resize(header.levels);
            }
        }

        // wtf is this formula???
        const auto idx = _worldx * CHUNKGRID_WIDTH + _worldy;
        const auto formula = 4 + idx * 8;
        stream.seekg(formula, std::ios::cur);
        STREAM_READ(stream, pack.offset);

        int skip = 0;
        for (uint32_t z = 0; z < header.levels; z++) {
            for (int x = 0; x < 10; x++) {
                for (int y = 0; y < 10; y++) {
                    if (skip > 0) {
                        --skip;
                        continue;
                    }

                    int32_t count;
                    STREAM_READ(stream, count);
                    if (count == -1) {
                        STREAM_READ(stream, skip);
                        if (skip > 0) {
                            --skip;
                        }
                        continue;
                    }

                    uint32_t room_id;
                    STREAM_READ(stream, room_id);
                    assert(count > 1 && count < 30);

                    if (count > 1) {
                        pack.offset_in_data[offset] = pack.data.size();
                        pack.data.emplace_back(count - 1);

                        int32_t another_int;
                        STREAM_READ(stream, another_int);

                        for (int i = 1; i < another_int; i++) {
                            int32_t more_int;
                            STREAM_READ(stream, more_int);
                            pack.data.emplace_back(more_int);
                        }
                    }
                }
            }
        }

        stream.close();
        _lotpacks.emplace_back(pack);
        return true;
    }


    bool read_lotheader(const fs::path &in) {
        std::ifstream stream(in, std::ios::binary);
        if (!stream.is_open() || !stream.good()) {
            spdlog::error("Failed to open file {}", in.string());
            return false;
        }

        // We return this later.
        auto header = lotheader();

        STREAM_READ(stream, header.version);
        STREAM_READ(stream, header.tile_count);
        for (uint32_t i = 0; i < header.tile_count; ++i) {
            std::string name;
            getline(stream, name);
            header.tiles.emplace_back(name);
        }
        // Skip 0x00 byte because it's always 0x00 and not used.
        stream.seekg(1, std::ios::cur);
        STREAM_READ(stream, header.width);
        STREAM_READ(stream, header.height);
        STREAM_READ(stream, header.levels);
        STREAM_READ(stream, header.room_count);
        for (uint32_t i = 0; i < header.room_count; ++i) {
            auto rdef = roomdef();
            std::string name;
            getline(stream, name);
            rdef.name = name;
            STREAM_READ(stream, rdef.level);
            STREAM_READ(stream, rdef.roomrect_count);
            for (uint32_t j = 0; j < rdef.roomrect_count; ++j) {
                auto rrect = roomrect();
                STREAM_READ(stream, rrect.x);
                STREAM_READ(stream, rrect.y);
                STREAM_READ(stream, rrect.width);
                STREAM_READ(stream, rrect.height);
                rrect.x += _worldx * CELL_WIDTH;
                rrect.y += _worldy * CELL_HEIGHT;
                rdef.roomrects.emplace_back(rrect);
            }
            header.rooms.emplace_back(rdef);

            STREAM_READ(stream, rdef.objects_count);
            // TODO: Are metaobjects unused?
            for (uint32_t j = 0; j < rdef.objects_count; ++j) {
                auto metaobj = metaobject();
                STREAM_READ(stream, metaobj.type);
                STREAM_READ(stream, metaobj.x);
                STREAM_READ(stream, metaobj.y);
                // metaobj.x += _worldx * CELL_WIDTH - rdef.x;
                // metaobj.y += _worldx * CELL_WIDTH - rdef.y;
            }
        }

        STREAM_READ(stream, header.building_count);
        for (uint32_t i = 0; i < header.building_count; ++i) {
            auto bdef = buildingdef();
            STREAM_READ(stream, bdef.room_count);

            // Some weird cyclic shit, black magic really.
            for (uint32_t j = 0; j < bdef.room_count; ++j) {
                uint32_t room_id;
                STREAM_READ(stream, room_id);
                auto rdef = header.rooms[room_id];
                rdef.building = bdef;
                bdef.rooms.emplace_back(rdef);
            }

            header.buildings.emplace_back(bdef);
        }

        // Read the zombie intensity into a 30x30 matrix.
        for (int x = 0; x < CHUNKGRID_WIDTH; x++) {
            for (int y = 0; y < CHUNKGRID_WIDTH; y++) {
                STREAM_READSZ(stream, header.intensity[x * CHUNKGRID_WIDTH + y], sizeof(uint32_t));
            }
        }

        stream.close();
        _lotheaders.emplace_back(header);
        return true;
    }
}
