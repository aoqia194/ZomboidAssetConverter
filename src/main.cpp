#include "main.hpp"

#include "asset.hpp"
#include "cxxopts.hpp"
#include "map.hpp"

#include "assimp/DefaultLogger.hpp"
#include "assimp/Logger.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/stopwatch.h"
#include "spdlog/sinks/stdout_color_sinks.h"

int main(const int argc, const char **argv)
{
    set_default_logger(spdlog::stdout_color_mt(PROJECT_NAME));
    spdlog::set_pattern(LOGGER_PATTERN);
    spdlog::enable_backtrace(5);
    spdlog::set_level(spdlog::level::info);

    // Used to provide default locations for saving assets.
    const fs::path exe_dir = fs::path(argv[0]).parent_path();

    cxxopts::Options options(PROJECT_NAME, PROJECT_DESC);
    options.add_options()
        ("v,verbose", "Enable verbose log output")
        ("h,help", "Print program usage")
        ("i,input", "Path to input directory", cxxopts::value<std::string>())
        ("o,output", "(optional) Path to output directory",
         cxxopts::value<std::string>()->default_value((exe_dir / "out").string())
            );
    options.add_options("asset")
        ("a,fix-assets", "Fix input asset files so that converter can work properly")
        ("A,convert-assets", "Convert assets to GLTF");
    options.add_options("maps")
        ("m,convert-maps", "Convert maps to PZW (and friends) for WorldEd");
    options.parse_positional({"input"});
    const auto result = options.parse(argc, argv);

    if (result["verbose"].as<bool>()) {
        spdlog::set_level(spdlog::level::trace);
        spdlog::disable_backtrace();
        Assimp::DefaultLogger::create("assimp.log", Assimp::Logger::VERBOSE);
    }

    if (result["help"].as<bool>()) {
        spdlog::info(options.help());
        return 0;
    }

    if (result.count("input") == 0) {
        spdlog::info(options.help());
        return 1;
    }

    in_dir = fs::path(result["input"].as<std::string>());
    out_dir = fs::path(result["output"].as<std::string>());

    spdlog::info("Looking for assets in {}", in_dir.string());

    const fs::path out_fixed_dir = out_dir / "fixed";
    const fs::path out_assets_dir = out_dir / "assets";
    const fs::path out_maps_dir = out_dir / "maps";

    if (result["fix-assets"].as<bool>()) {
        spdlog::stopwatch sw;

        if (fs::exists(out_fixed_dir)) {
            spdlog::info("Found previously fixed files. Deleting the fixed folder.");
            fs::remove_all(out_fixed_dir);
        }

        for (const auto &entry : fs::recursive_directory_iterator(in_dir)) {
            const auto &entry_path = entry.path();
            const auto &entry_ext = entry_path.extension().string();
            const auto &parent_stem = entry_path.parent_path().stem();

            spdlog::trace("Recursing and fixing: {}", entry_path.string());

            if (!entry.is_regular_file() || parent_stem == "fixed" ||
                parent_stem == "assets" || (entry_ext != ".X" && entry_ext != ".x")) {
                continue;
            }

            const auto out = out_fixed_dir / fs::relative(entry_path, in_dir);
            if (!asset::fix(entry_path, out)) {
                spdlog::error("Failed to fix all assets. Stopped to prevent issues.");
                spdlog::dump_backtrace();
                return 1;
            }
        }
        spdlog::info("Fixing assets took {}ms", sw.elapsed_ms().count());
    }

    if (result["convert-assets"].as<bool>()) {
        const auto in = result["fix-assets"].as<bool>() ? out_fixed_dir : in_dir;
        if (in == out_fixed_dir && !exists(out_fixed_dir)) {
            spdlog::error("Fix assets was enabled but there are no fixed assets.");
            spdlog::error("Exiting to prevent undefined behaviour.");

            return 1;
        }

        if (fs::exists(out_assets_dir)) {
            spdlog::info("Found previously converted files. Deleting the assets folder.");
            fs::remove_all(out_assets_dir);
        }

        spdlog::stopwatch sw;
        for (const auto &entry : fs::recursive_directory_iterator(in)) {
            if (!entry.is_regular_file())
                continue;

            const auto &entry_path = entry.path();
            const auto &entry_ext = entry_path.extension().string();
            spdlog::trace("Recursing and converting: {}", entry_path.string());

            if (entry_ext != ".X" && entry_ext != ".x")
                continue;

            const auto out_path = (out_assets_dir / fs::relative(entry_path, in))
                .replace_extension("gltf");

            if (!asset::load(entry_path) || !asset::dump("gltf2", out_path)) {
                spdlog::error("Failed to dump all assets. Stopped to prevent issues.");
                spdlog::dump_backtrace();
                return 1;
            }
        }
        spdlog::info("Converting assets took {}ms", sw.elapsed_ms().count());
    }

    if (result["convert-maps"].as<bool>()) {
        if (fs::exists(out_maps_dir)) {
            spdlog::info("Found previously converted files. Deleting the maps folder.");
            fs::remove_all(out_maps_dir);
        }

        spdlog::stopwatch sw;
        for (const auto &entry : fs::directory_iterator(in_dir)) {
            if (!entry.is_directory())
                continue;

            const auto &entry_path = entry.path();
            const auto &entry_stem = entry_path.stem().string();
            spdlog::trace("convert maps dir {}", entry.path().string());

            map::pzmap pzmap{};
            if (!pzmap.read(entry_path)) {
                spdlog::error("Failed to read map from {}", entry_path.string());
                return 1;
            }

            const auto &out_path = out_maps_dir / relative(entry, in_dir);

            const auto pzw_path = out_path / (entry_stem + ".pzw");
            if (!pzmap.write(pzw_path)) {
                spdlog::error("Failed to write PZW map file {}", pzw_path.string());
                return 1;
            }
        }

        spdlog::info("Converting maps took {:.2}s", sw);
        spdlog::info(
            "Don't forget to copy the tiles that the map uses to the 'tiles\\' folder!");
    }

    spdlog::info("<3 Completed! Have a nice day.");
    return 0;
}