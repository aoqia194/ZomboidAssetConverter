#include "main.h"

#include "cxxopts.hpp"
#include "gui.h"
#include "window.h"

#include "assimp\DefaultLogger.hpp"
#include "assimp\Logger.hpp"

#include "spdlog\spdlog.h"

extern fs::path exe_dir;
extern fs::path in_dir;
extern fs::path out_dir;
extern fs::path in_assets_dir;
extern fs::path out_fixed_dir;
extern fs::path out_assets_dir;

int main(const int argc, const char **argv) {
    spdlog::enable_backtrace(5);
    spdlog::set_level(spdlog::level::info);

    // Handle program args.

    cxxopts::Options options(PROJECT_NAME, PROJECT_DESC);
    options.add_options()
        ("h,help", "Print program usage")
        ("v,verbose", "Enable verbose log output")
        ("d,debug", "Enable debugging");
    const auto result = options.parse(argc, argv);

    if (result["verbose"].as<bool>()) spdlog::set_level(spdlog::level::trace);
    if (result["help"].as<bool>()) {
        spdlog::info(options.help());
        return 0;
    }
    if (result["debug"].as<bool>()) {
        Assimp::DefaultLogger::create("AssimpLog.txt", Assimp::Logger::VERBOSE);
    }

    exe_dir = fs::path(argv[0]).parent_path();
    in_dir = exe_dir / "in";
    out_dir = exe_dir / "out";
    in_assets_dir = in_dir / "assets";
    out_fixed_dir = out_dir / "fixed";
    out_assets_dir = out_dir / "assets";

    // Create window! The fun stuff begins :D
    return window::init();
}
