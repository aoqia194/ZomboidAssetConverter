#include "main.hpp"

#include "asset.hpp"
#include "cxxopts.hpp"
#include "map.hpp"

#include "assimp\DefaultLogger.hpp"
#include "assimp\Logger.hpp"

#include "spdlog\spdlog.h"
#include "spdlog\stopwatch.h"
#include "spdlog\sinks\stdout_color_sinks.h"

int main(const int argc, const char **argv) {
    set_default_logger(spdlog::stdout_color_mt(PROJECT_NAME));
    spdlog::enable_backtrace(5);
    spdlog::set_level(spdlog::level::info);

    // Handle program args.

    cxxopts::Options options(PROJECT_NAME, PROJECT_DESC);
    options.add_options()
        ("v,verbose", "Enable verbose log output")
        ("h,help", "Print program usage")
        ("i,input", "Path to input directory", cxxopts::value<std::string>())
        ("o,output", "(optional) Path to output directory", cxxopts::value<std::string>()->default_value(""));
    options.add_options("assets")
        ("f,fix-assets", "Fix input asset files")
        ("a,convert-assets", "Convert assets");
    options.add_options("maps")
        ("m,convert-maps", "Convert maps");
    options.parse_positional({ "input" });
    const auto result = options.parse(argc, argv);

    if (result["verbose"].as<bool>()) {
        spdlog::set_level(spdlog::level::trace);
        Assimp::DefaultLogger::create("assimp.log", Assimp::Logger::VERBOSE);
    }

    if (result["help"].as<bool>()) {
        spdlog::info(options.help());
        return 0;
    }
    const auto input = result["input"].as<std::string>();
    const auto output = result["output"].as<std::string>();
    const auto fix_assets = result["fix-assets"].as<bool>();
    const auto convert_assets = result["convert-assets"].as<bool>();
    const auto convert_maps = result["convert-maps"].as<bool>();

    // yummyyy
    const fs::path exe_dir = fs::path(argv[0]).parent_path();
    const fs::path in_dir = !input.empty() ? input : exe_dir / "in";
    const fs::path out_dir = !output.empty() ? output : exe_dir / "out";
    const fs::path out_fixed_dir = out_dir / "fixed";
    const fs::path out_assets_dir = out_dir / "assets";
    const fs::path out_maps_dir = out_dir / "maps";

    if ((fix_assets || convert_assets) && convert_maps) {
        spdlog::error("Cannot fix/convert assets at the same time as converting maps.");
        return 1;
    }

    // Fix the assets if needed.
    if (fix_assets) {
        spdlog::stopwatch sw;
        for (const auto &entry: fs::recursive_directory_iterator(in_dir)) {
            if (!entry.is_regular_file()) continue;
            const auto &entry_path = entry.path();
            const auto out = out_fixed_dir / relative(entry_path, in_dir);

            if (!asset::fix(entry_path, out)) {
                spdlog::error("Failed to fix all assets. Stopped to prevent issues.");
                return 1;
            }
        }
        spdlog::info("Fixing assets took {:.2}s", sw);
    }

    // Convert the assets if needed.
    if (convert_assets) {
        const auto in = fix_assets ? out_fixed_dir : in_dir;

        spdlog::stopwatch sw;
        for (const auto &entry: fs::recursive_directory_iterator(in)) {
            if (!entry.is_regular_file()) continue;
            const auto &entry_path = entry.path();
            const auto out_path = (out_assets_dir / relative(entry_path, in)).replace_extension("gltf");

            if (!asset::load(entry_path) || !asset::dump("gltf2", out_path)) {
                spdlog::error("Failed to dump all assets. Stopped to prevent issues.");
                return 1;
            }
        }
        spdlog::info("Converting assets took {:.2}s", sw);
    }

    // Convert the maps if needed.
    if (convert_maps) {
        spdlog::stopwatch sw;

        for (const auto &map: fs::directory_iterator(in_dir)) {
            if (!map.is_directory()) continue;
            const auto &map_path = map.path();
            const auto &map_name = map_path.stem().string();
            const auto out_path = out_maps_dir / relative(map, in_dir);
            spdlog::trace("dir {}", map.path().string());

            for (const auto &entry: fs::directory_iterator(map)) {
                if (!entry.is_regular_file()) continue;

                const auto &entry_path = entry.path();
                if (entry_path.extension() != ".lotheader") continue;

                const auto &entry_stem = entry_path.stem().string();
                const auto p = entry_stem.find_first_of('_');

                const auto cell_x = std::stoul(entry_stem.substr(0, p));
                const auto cell_y = std::stoul(entry_stem.substr(p + 1));
                map::_worldx = cell_x;
                map::_worldy = cell_y;

                if (cell_x >= map::_width) map::_width = cell_x + 1;
                if (cell_y >= map::_height) map::_height = cell_y + 1;

                const auto rel_path = relative(entry_path, in_dir);
                spdlog::debug("Found map cell {}", rel_path.string());

                const auto header_ret = map::read_lotheader(entry_path);
                if (!header_ret) {
                    spdlog::error("Failed to read lotheader file {}", entry_path.string());
                    return 1;
                }

                const auto lotpack_file = entry_path.parent_path() / std::format("world_{}_{}.lotpack", cell_x, cell_y);
                const auto pack_ret = map::read_lotpack(lotpack_file);
                if (!pack_ret) {
                    spdlog::error("Failed to read lotpack file {}", lotpack_file.string());
                    return 1;
                }

                map::_index++;
            }

            const auto pzw_path = out_path / (map_name + ".pzw");
            const auto pzw_ret = map::write_pzw(pzw_path);
            if (!pzw_ret) {
                spdlog::error("Failed to write PZW map file {}", pzw_path.string());
                return 1;
            }
        }

        spdlog::info("Converting maps took {:.2}s", sw);
        spdlog::info("Don't forget to copy the tiles that the map uses to the 'tiles\\' folder!");
    }

    spdlog::info("💖 Completed! Have a nice day.");
    return 0;
}
