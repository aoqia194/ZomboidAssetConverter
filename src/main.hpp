#ifndef MAIN_HPP
#define MAIN_HPP

#define PROJECT_NAME "ZomboidAssetConverter"
#define PROJECT_DESC \
    "Imports, optionally fixes, and exports Zomboid .X files. Imports and exports Zomboid maps."

#define LOGGER_PATTERN "[%Y-%m-%d %H:%M:%S] [%^%l%$] %v"

#include <filesystem>

#include "spdlog/logger.h"

namespace fs = std::filesystem;

#endif //MAIN_HPP
