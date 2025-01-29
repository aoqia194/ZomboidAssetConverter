#ifndef ASSET_H
#define ASSET_H

#include "main.h"

#include "assimp\scene.h"

namespace asset {
    inline std::shared_ptr<const aiScene *> _scene;

    /**
     * Dumps the loaded scene to a file.
     * @param format The format to pass to assimp.
     * @param out The file to write to.
     * @return True if the dump was successful.
     */
    bool dump(const char *format, const fs::path &out);

    /**
     * Loads an asset into an Assimp::aiScene.
     * @param in The absolute path to the asset.
     * @return True if the scene loaded properly.
     */
    bool load(const fs::path &in);

    /**
     * Fixes a file.
     * @param in The path to the file to fix.
     * @param out The path for the fixed file.
     * @return True if the file was fixed successfully.
     */
    bool fix(const fs::path &in, const fs::path &out);
}

#endif //ASSET_H
