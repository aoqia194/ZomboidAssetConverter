#ifndef MAIN_H
#define MAIN_H

#define PROJECT_NAME "ZomboidAssetConverter"
#define PROJECT_DESC "Imports, optionally fixes, and exports Zomboid .X files."

#include <filesystem>

namespace fs = std::filesystem;

inline fs::path exe_dir;
inline fs::path in_dir;
inline fs::path out_dir;
inline fs::path in_assets_dir;
inline fs::path out_fixed_dir;
inline fs::path out_assets_dir;

#endif //MAIN_H
