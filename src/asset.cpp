#include "asset.hpp"

#include <fstream>
#include <regex>

#include "assimp/Exporter.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "spdlog/spdlog.h"

namespace asset {
    const std::regex BROKEN_XANIM_REGEX(R"(^\s*;)");

    const auto _importer = std::make_shared<Assimp::Importer>();
    const auto _exporter = std::make_shared<Assimp::Exporter>();

    bool dump(const char *format, const fs::path &out) {
        create_directories(out.parent_path());

        SPDLOG_DEBUG("Exporting asset scene to {}", out.string());
        const auto res = _exporter->Export(*_scene, format, out.string(), aiProcess_ValidateDataStructure);
        if (res != AI_SUCCESS) {
            spdlog::error("❌ Failed to export scene: {}", _exporter->GetErrorString());
        }
        SPDLOG_INFO("Saved exported file at {}", out.string());

        // SHOULDN'T need to explicitly reset because the scene SHOULD get deleted when this goes out of scope.
        // But we do because idk why the smart pointer isn't resetting automatically.
        SPDLOG_DEBUG("Resetting assimp scene.");
        _scene.reset();

        return res == AI_SUCCESS;
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
            return false;
        }

        return true;
    }

    bool fix(const fs::path &in, const fs::path &out) {
        if (!exists(in)) {
            SPDLOG_ERROR("Asset {} does not exist", in.string());
            return false;
        }

        auto infile = std::ifstream(in);
        if (!infile.is_open()) {
            SPDLOG_ERROR("Could not open asset {}", in.string());
            return false;
        }

        std::stringstream buf;
        std::string line;
        std::smatch m;

        int i = 0;
        while (getline(infile, line)) {
            if (std::regex_match(line, m, BROKEN_XANIM_REGEX)) {
                SPDLOG_DEBUG("Found bugged DirectX format at line {}.", i);
            } else {
                buf << line + "\n";
            }

            i++;
        }
        infile.close();

        create_directories(out.parent_path());

        auto outfile = std::ofstream(out);
        if (!outfile.is_open()) {
            SPDLOG_ERROR("Could not open output file {}", out.string());
            return false;
        }

        outfile << buf.str();
        outfile.close();

        SPDLOG_INFO("Saved fixed file at {}", out.string());
        return true;
    }
}
