#ifndef MAIN_HPP
#define MAIN_HPP

#define PROJECT_NAME "ZomboidAssetConverter"
#define PROJECT_DESC \
    "Imports, optionally fixes, and exports Zomboid .X files. Imports and exports Zomboid maps."

#define LOGGER_PATTERN "[%Y-%m-%d %H:%M:%S] [%^%l%$] %v"

#include <filesystem>

#include "spdlog/logger.h"

namespace fs = std::filesystem;

#define CELL_WIDTH 300
#define CELL_HEIGHT 300
#define CHUNK_WIDTH 10
#define CHUNK_HEIGHT 10
#define CHUNK_MAXLEVELS 16
#define CHUNKGRID_SIZE 30

#endif //MAIN_HPP
