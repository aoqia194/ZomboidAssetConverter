#include "map.hpp"

#include <fstream>

#include "pugixml.hpp"

#include "map\pzw.hpp"

#include "spdlog\spdlog.h"

#define STREAM_READ(stream, out) stream.read(reinterpret_cast<char *>(&out), sizeof(out))
#define STREAM_READSZ(stream, out, size) stream.read(reinterpret_cast<char *>(&out), size)

namespace pz {
    bool map::write(const fs::path &out) {
        // TODO: Loop through all of the required lists becacuse these are only 1 file per call.
        // return write_tbx(out) && write_tmx(out) && write_pzw(out);
        return write_tbx(out);
    }


    bool map::read(const fs::path &in) {
        this->name = in.filename().string();

        // Do a single pass, getting the last lotheader file to determine the
        // map's width and height in cells. Probably could've improved this but im tired.
        for (const auto &entry: fs::directory_iterator(in)) {
            if (!entry.is_regular_file()) continue;
            const auto &path = entry.path();
            if (path.extension() != ".lotheader") continue;
            const auto &stem = path.stem().string();
            const auto p = stem.find_first_of('_');
            const uint32_t wx = std::stoi(stem.substr(0, p));
            const uint32_t wy = std::stoi(stem.substr(p + 1));

            if (this->worldx == 0) this->worldx = wx;
            if (this->worldy == 0) this->worldy = wy;

            if (wx >= this->worldx) this->width = wx - this->worldx + 1;
            if (wy >= this->worldy) this->height = wy - this->worldy + 1;
        }

        // Resize vectors to save a little performance.
        // And because the inner vectors need to be initialised properly.
        this->lotheaders.resize(this->width);
        this->lotpacks.resize(this->width);
        // for (auto &header: this->lotheaders) header.resize(this->height);
        // for (auto &pack: this->lotpacks) pack.resize(this->height);

        for (const auto &entry: fs::directory_iterator(in)) { // NOLINT(*-use-anyofallof)
            if (!entry.is_regular_file()) continue;

            const auto &path = entry.path();
            // We only need to loop through the lotheader files, because we can infer the
            // other file names from just the cellX and cellY of the filename.
            if (path.extension() != ".lotheader") continue;
            const auto &stem = path.stem().string();
            const auto p = stem.find_first_of('_');
            const auto wx = std::stoi(stem.substr(0, p));
            const auto wy = std::stoi(stem.substr(p + 1));

            const auto rel_path = relative(path, in_dir);
            spdlog::debug("Found map cell {}", rel_path.string());

            const auto header_ret = read_lotheader(path, wx, wy);
            if (!header_ret) {
                spdlog::error("Failed to read lotheader file {}", path.string());
                return false;
            }

            const auto lotpack_file = path.parent_path() / std::format(
                                          "world_{}_{}.lotpack", wx, wy);
            const auto pack_ret = read_lotpack(lotpack_file, wx, wy);
            if (!pack_ret) {
                spdlog::error("Failed to read lotpack file {}", lotpack_file.string());
                return false;
            }
        }

        return true;
    }

    bool map::write_pzw(const fs::path &out) {
        pugi::xml_document doc;

        auto world = doc.append_child("world");
        world.append_attribute("version") = "1.0";
        world.append_attribute("width") = this->width;
        world.append_attribute("height") = this->height;

        pzw::add_propertyenums(doc);
        pzw::add_propertydefs(doc);
        // TODO: Finish.
    }

    bool map::write_tbx(const fs::path &out, buildingdef* building) {
        pugi::xml_document doc;

        auto building = doc.append_child("building");
        building.append_attribute("version") = 3;

        return true;
    }

    bool map::read_lotpack(const fs::path &in, const uint32_t wx, const uint32_t wy) {
        const auto fwx = wx / CHUNKGRID_SIZE;
        const auto fwy = wy / CHUNKGRID_SIZE;
        const auto cx = wx - this->worldx;
        const auto cy = wy - this->worldy;

        std::ifstream stream(in, std::ios::binary);
        if (!stream.is_open() || !stream.good()) {
            spdlog::error("Failed to open file {}", in.string());
            return false;
        }

        lotheader *header = &this->lotheaders[cx][cy];
        auto pack = lotpack(wx, wy, header);

        // wtf is this formula???
        const auto lwx = wx - fwx * CHUNKGRID_SIZE;
        const auto lwy = wy - fwy * CHUNKGRID_SIZE;
        const auto idx = lwx * CHUNKGRID_SIZE + lwy;

        stream.seekg(4 + idx * 8, std::ios::beg);
        STREAM_READ(stream, pack.offset);
        stream.seekg(pack.offset, std::ios::beg); // TODO: Is this from cur or beg?

        int skip = 0;
        for (uint32_t z = 0; z < header->levels; ++z) {
            for (int x = 0; x < CHUNK_WIDTH; ++x) {
                for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                    if (skip > 0) {
                        --skip;
                        continue;
                    }

                    int32_t count;
                    STREAM_READ(stream, count);
                    if (count == -1) {
                        STREAM_READ(stream, skip);
                        if (skip > 0) --skip;
                        continue;
                    }

                    uint32_t room_id;
                    STREAM_READ(stream, room_id);
                    pack.room_ids[x][y][z] = room_id;
                    assert(count > 1 && count < 30);

                    for (auto i = 1; i < count; ++i) {
                        int32_t data; // I think it can be neg (-1) here.
                        STREAM_READ(stream, data);
                        pack.data[x][y][z].emplace_back(data);
                    }

                    // if (count > 1) {
                    //     pack.offset_in_data[pack.offset] = pack.data.size();
                    //     pack.data.emplace_back(count - 1);
                    //
                    //     int32_t another_int;
                    //     STREAM_READ(stream, another_int);
                    //
                    //     for (int i = 1; i < another_int; i++) {
                    //         int32_t more_int;
                    //         STREAM_READ(stream, more_int);
                    //         pack.data.emplace_back(more_int);
                    //     }
                    // }
                }
            }
        }

        stream.close();
        this->lotpacks[cx].emplace_back(pack);
        return true;
    }

    bool map::read_lotheader(const fs::path &in, const uint32_t wx, const uint32_t wy) {
        std::ifstream stream(in, std::ios::binary);
        if (!stream.is_open() || !stream.good()) {
            spdlog::error("Failed to open file {}", in.string());
            return false;
        }

        // We return this later.
        auto header = lotheader(wx, wy);

        STREAM_READ(stream, header.version);
        STREAM_READ(stream, header.tile_count);
        for (uint32_t i = 0; i < header.tile_count; ++i) {
            std::string tile;
            getline(stream, tile);
            header.tiles.emplace_back(tile);
        }
        stream.seekg(1, std::ios::cur); // Skip 0x00 byte because it's always 0x00 and not used.
        STREAM_READ(stream, header.width);
        STREAM_READ(stream, header.height);
        STREAM_READ(stream, header.levels);
        STREAM_READ(stream, header.room_count);
        for (uint32_t i = 0; i < header.room_count; ++i) {
            auto rdef = roomdef();
            std::string rname;
            getline(stream, rname);
            rdef.name = rname;
            STREAM_READ(stream, rdef.level);
            STREAM_READ(stream, rdef.roomrect_count);
            for (uint32_t j = 0; j < rdef.roomrect_count; ++j) {
                auto rrect = roomrect();
                STREAM_READ(stream, rrect.x);
                STREAM_READ(stream, rrect.y);
                STREAM_READ(stream, rrect.width);
                STREAM_READ(stream, rrect.height);
                rrect.x += wx * CELL_WIDTH;
                rrect.y += wy * CELL_HEIGHT;
                rdef.roomrects.emplace_back(rrect);
            }
            header.rooms.emplace_back(rdef);

            STREAM_READ(stream, rdef.objects_count);
            // TODO: Are metaobjects unused?
            for (uint32_t j = 0; j < rdef.objects_count; ++j) {
                // ReSharper disable once CppUseStructuredBinding
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

            // Linking the building back to the room that it belongs to.
            for (uint32_t j = 0; j < bdef.room_count; ++j) {
                uint32_t room_id;
                STREAM_READ(stream, room_id);
                auto rdef = header.rooms[room_id];
                rdef.building = &bdef;
                bdef.rooms.emplace_back(rdef);
            }

            header.buildings.emplace_back(bdef);
        }

        // Read the zombie intensity into a 300x300 matrix. Each lot has it's own one.
        header.intensity.resize(CHUNKGRID_SIZE * CHUNKGRID_SIZE);
        for (uint32_t x = 0; x < CHUNKGRID_SIZE; x++) {
            for (uint32_t y = 0; y < CHUNKGRID_SIZE; y++) {
                // TODO: Original per-pixel formula does cellX*30+x so do we maybe do cellX/30-x?
                STREAM_READSZ(stream, header.intensity[x * CHUNKGRID_SIZE + y], sizeof(uint8_t));
            }
        }

        stream.close();
        this->lotheaders[wx - this->worldx].emplace_back(header);
        return true;
    }
}
