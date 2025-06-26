#include "asset.hpp"

#include <fstream>
#include <regex>

#include "assimp/Exporter.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "spdlog/spdlog.h"

namespace asset {
    const std::regex BROKEN_XANIM_REGEX(R"(^\\s*;)");

    const auto _importer = std::make_shared<Assimp::Importer>();
    const auto _exporter = std::make_shared<Assimp::Exporter>();

    bool dump(const char *format, const fs::path &out) {
        create_directories(out.parent_path());

        SPDLOG_DEBUG("Exporting asset scene to {}", out.string());
        const auto res = _exporter->Export(*_scene, format, out.string(),
                                           aiProcess_ValidateDataStructure);
        if (res != aiReturn_SUCCESS) {
            SPDLOG_ERROR("Failed to export scene: {}", _exporter->GetErrorString());
        }
        SPDLOG_INFO("Saved exported file at {}", out.string());

        // SHOULDN'T need to explicitly reset because the scene SHOULD get deleted when this goes out of scope.
        // But we do because idk why the smart pointer isn't resetting automatically.
        SPDLOG_DEBUG("Resetting assimp scene.");
        _scene.reset();

        return res == aiReturn_SUCCESS;
    }

    bool load(const fs::path &in) {
        auto path_str = in.string();
        if (in.empty() || !exists(in)) {
            SPDLOG_WARN("Asset {} does not exist.", path_str);
            return false;
        }

        SPDLOG_DEBUG("Loading scene for asset: {}", path_str);

        if (_scene != nullptr) {
            SPDLOG_WARN("Previously loaded scene found. It WILL get discarded!");
        }

        _scene = std::make_shared<const aiScene *>(_importer->ReadFile(
            path_str,
            aiProcess_ValidateDataStructure | aiProcess_FindInvalidData | aiProcess_GlobalScale |
            aiProcess_SortByPType));
        if (_scene == nullptr || (*_scene)->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
            SPDLOG_ERROR("Failed to load asset at {}: {}", path_str, _importer->GetErrorString());
            spdlog::dump_backtrace();
            return false;
        }

        return true;
    }

    bool fix(const fs::path &in, const fs::path &out) {
        if (!exists(in)) {
            SPDLOG_ERROR("Asset {} does not exist", in.string());
            spdlog::dump_backtrace();
            return false;
        }

        create_directories(out.parent_path());

        auto infile = std::ifstream(in, std::ios::binary);
        if (!infile.is_open()) {
            SPDLOG_ERROR("Could not open asset {}", in.string());
            spdlog::dump_backtrace();
            return false;
        }

        auto outfile = std::ofstream(out, std::ios::binary);
        if (!outfile.is_open()) {
            SPDLOG_ERROR("Could not open output file {}", out.string());
            spdlog::dump_backtrace();
            return false;
        }

        std::string line;

        // Skip fixing if DirectX format is binary.
        std::getline(infile, line);
        outfile << line << std::endl;
        if (line.substr(8, 3) == "bin") {
            SPDLOG_DEBUG("DirectX file was binary format, no need to fix.");
            outfile << infile.rdbuf();
        } else {
            int i = 0;
            while (std::getline(infile, line)) {
                if (std::regex_match(line, BROKEN_XANIM_REGEX)) {
                    SPDLOG_DEBUG("Found bugged DirectX format at line {}.", i);
                } else {
                    outfile << line << std::endl;
                }

                i++;
            }
        }

        infile.close();
        outfile.close();

        SPDLOG_INFO("Saved fixed file at {}", out.string());
        return true;
    }
}
