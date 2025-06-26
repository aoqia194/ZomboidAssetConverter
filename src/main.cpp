#include "main.hpp"

#include "asset.hpp"
#include "cxxopts.hpp"

#include "assimp/DefaultLogger.hpp"
#include "assimp/Logger.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/stopwatch.h"
#include "spdlog/sinks/stdout_color_sinks.h"

int main(const int argc, const char** argv) {
    set_default_logger(spdlog::stdout_color_mt(PROJECT_NAME));
    spdlog::set_pattern(LOGGER_PATTERN);
    spdlog::enable_backtrace(5);
    spdlog::set_level(spdlog::level::info);

    // Set the executable path directory; used to provide default locations for saving assets
    const fs::path exe_dir = fs::path(argv[0]).parent_path();

    // Handle program args.
    cxxopts::Options options(PROJECT_NAME, PROJECT_DESC);
    options.add_options()
        ("v,verbose", "Enable verbose log output")
        ("h,help", "Print program usage");
    options.add_options("asset")
        ("i,input", "Path to input directory", cxxopts::value<std::string>())
        (
            "o,output", "(optional) Path to output directory",
            cxxopts::value<std::string>()->default_value((exe_dir / "out").string())
        )
        ("a,fix-assets", "Fix input asset files")
        ("A,convert-assets", "Convert assets");
    options.parse_positional({ "input" });
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

    const auto in_dir = fs::path(result["input"].as<std::string>());
    const auto out_dir = fs::path(result["output"].as<std::string>());

    spdlog::info("Looking for assets in {}", in_dir.string());

    const auto out_fixed_dir = out_dir / "fixed";
    if (fs::exists(out_fixed_dir) && !out_fixed_dir.empty()) {
        spdlog::warn("Found non-empty fixed directory. Potential for recursive conversion.");
    }

    const auto out_assets_dir = out_dir / "assets";
    if (fs::exists(out_assets_dir) && !out_assets_dir.empty()) {
        spdlog::warn("Found non-empty fixed directory. Potential for recursive conversion.");
    }

    // Fix the assets if needed.
    if (result["fix-assets"].as<bool>()) {
        spdlog::stopwatch sw;
        for (const auto& entry: fs::recursive_directory_iterator(in_dir)) {
            spdlog::trace("Recursing and fixing through directory {}", entry_path.string());

            if (!entry.is_regular_file()) continue;
            const auto& entry_path = entry.path();
            const auto out = out_fixed_dir / fs::relative(entry_path, in_dir);

            if (!asset::fix(entry_path, out)) {
                spdlog::error("Failed to fix all assets. Stopped to prevent issues.");
                spdlog::dump_backtrace();
                return 1;
            }
        }
        spdlog::info("Fixing assets took {}ms", sw.elapsed_ms().count());
    }

    // Convert the assets if needed.
    if (result["convert-assets"].as<bool>()) {
        const auto in = result["fix-assets"].as<bool>() ? out_fixed_dir : in_dir;

        spdlog::stopwatch sw;
        for (const auto& entry: fs::recursive_directory_iterator(in)) {
            const auto& entry_path = entry.path();
            spdlog::trace("Recursing and fixing through directory {}", entry_path.string());

            const auto& entry_ext = entry_path.extension().string();
            if (!entry.is_regular_file()) continue;
            if (entry_ext != ".X" || entry_ext != ".x") continue;
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

    spdlog::info("<3 Completed! Have a nice day.");
    return 0;
}
