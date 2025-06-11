#include "main.h"

#include "asset.h"
#include "cxxopts.hpp"

#include "assimp/DefaultLogger.hpp"
#include "assimp/Logger.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/stopwatch.h"
#include "spdlog/sinks/stdout_color_sinks.h"

int main(const int argc, const char **argv) {
    set_default_logger(spdlog::stdout_color_mt(PROJECT_NAME));
    spdlog::enable_backtrace(5);
    spdlog::set_level(spdlog::level::info);

    // Handle program args.

    cxxopts::Options options(PROJECT_NAME, PROJECT_DESC);
    options.add_options()
        ("v,verbose", "Enable verbose log output")
        ("h,help", "Print program usage")
        ("i,input", "Path to input directory", cxxopts::value<std::string>()->default_value(""))
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
        SPDLOG_INFO(options.help());
        return 0;
    }

    if (result["input"].as<std::string>().empty()) {
        SPDLOG_ERROR("No input directory specified!  Please pass the directory containing your .x files as the last parameter");
        SPDLOG_INFO(options.help());
        return 1;
    }

    if (!result["fix-assets"].as<bool>() && !result["convert-assets"].as<bool>()) {
        SPDLOG_ERROR("No action requested!  Please include -a and/or -f");
        SPDLOG_INFO(options.help());
        return 1;
    }

    //const std::string input = "";
    //const std::string output = "";
    const auto input = result["input"].as<std::string>();
    const auto output = result["output"].as<std::string>();

    // yummyyy
    const fs::path exe_dir = fs::path(argv[0]).parent_path();
    fs::path out_dir = exe_dir / "out";
    const fs::path in_dir = std::filesystem::u8path(input);

    if (!output.empty()) {
        out_dir = std::filesystem::u8path(output);
    }

    SPDLOG_INFO("Looking for assets in {}", in_dir.c_str());

    const fs::path out_fixed_dir = out_dir / "fixed";
    const fs::path out_assets_dir = out_dir / "assets";

    // Fix the assets if needed.
    if (result["fix-assets"].as<bool>()) {
        spdlog::stopwatch sw;
        for (const auto &entry: fs::recursive_directory_iterator(in_dir)) {
            if (!entry.is_regular_file()) continue;
            const auto &entry_path = entry.path();
            const auto out = out_fixed_dir / relative(entry_path, in_dir);

            if (!asset::fix(entry_path, out)) {
                SPDLOG_ERROR("Failed to fix all assets. Stopped to prevent issues.");
                return 1;
            }
        }
        SPDLOG_INFO("Fixing assets took {:.2}s", sw);
    }

    // Convert the assets if needed.
    if (result["convert-assets"].as<bool>()) {
        const auto in = result["fix-assets"].as<bool>() ? out_fixed_dir : in_dir;

        spdlog::stopwatch sw;
        for (const auto &entry: fs::recursive_directory_iterator(in)) {
            if (!entry.is_regular_file()) continue;
            const auto &entry_path = entry.path();
            const auto out_path = (out_assets_dir / relative(entry_path, in)).replace_extension("gltf");

            if (!asset::load(entry_path) || !asset::dump("gltf2", out_path)) {
                SPDLOG_ERROR("Failed to dump all assets. Stopped to prevent issues.");
                return 1;
            }
        }
        SPDLOG_INFO("Converting assets took {:.2}s", sw);
    }

    SPDLOG_INFO("💖 Completed! Have a nice day.");
    return 0;
}
