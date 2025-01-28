#include "gui.h"

#include <fstream>

#include "asset.h"
#include "imgui.h"
#include "main.h"
#include "window.h"

#include "boost\lexical_cast.hpp"

#include "spdlog\spdlog.h"

#define STR_CAST(value) (boost::lexical_cast<std::string>(value))
#define VEC_STR(vec) (std::format("({}, {}, {})", vec.x, vec.y, vec.z))

const char *behaviour_string(const aiAnimBehaviour i) {
    switch (i) {
        case aiAnimBehaviour_DEFAULT: return "DEFAULT";
        case aiAnimBehaviour_CONSTANT: return "CONSTANT";
        case aiAnimBehaviour_LINEAR: return "LINEAR";
        case aiAnimBehaviour_REPEAT: return "REPEAT";
        default: return "ERR";
    }
}

// Taken from tracy :)
namespace ImGuiUtil {
    namespace Style::Colours {
        constexpr static ImU32 ImGuiCol_TextDisabled = IM_COL32(127, 127, 127, 255);
    }

    [[maybe_unused]] static void TextDisabledUnformatted(const char *begin, const char *end = nullptr) {
        ImGui::PushStyleColor(ImGuiCol_Text, Style::Colours::ImGuiCol_TextDisabled);
        ImGui::TextUnformatted(begin, end);
        ImGui::PopStyleColor();
    }

    [[maybe_unused]] static void TextFocused(const char *label, const char *value) {
        TextDisabledUnformatted(label);
        ImGui::SameLine();
        ImGui::TextUnformatted(value);
    }
}

namespace gui {
    void scene_window() {
        ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT));
        ImGui::Begin("Loaded Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        if (asset::scene::instance == nullptr) {
            ImGui::Text("Waiting for loaded assimp scene...");
            ImGui::End();
            return;
        }

        const auto scene = *asset::scene::instance;
        ImGuiUtil::TextFocused("Scene Name: ", scene->mName.C_Str());
        ImGuiUtil::TextFocused("Total Memory: ", STR_CAST(asset::scene::meminfo.total).c_str());
        ImGuiUtil::TextFocused("Nodes: ", STR_CAST(scene->mNumAnimations).c_str());
        ImGui::SameLine();
        ImGuiUtil::TextFocused("Memory: ", STR_CAST(asset::scene::meminfo.animations).c_str());
        ImGuiUtil::TextFocused("Animations: ", STR_CAST(scene->mNumAnimations).c_str());
        ImGui::SameLine();
        ImGuiUtil::TextFocused("Memory: ", STR_CAST(asset::scene::meminfo.animations).c_str());
        ImGuiUtil::TextFocused("Cameras: ", STR_CAST(scene->mNumCameras).c_str());
        ImGui::SameLine();
        ImGuiUtil::TextFocused("Memory: ", STR_CAST(asset::scene::meminfo.cameras).c_str());
        ImGuiUtil::TextFocused("Lights: ", STR_CAST(scene->mNumLights).c_str());
        ImGui::SameLine();
        ImGuiUtil::TextFocused("Memory: ", STR_CAST(asset::scene::meminfo.lights).c_str());
        ImGuiUtil::TextFocused("Materials: ", STR_CAST(scene->mNumMaterials).c_str());
        ImGui::SameLine();
        ImGuiUtil::TextFocused("Memory: ", STR_CAST(asset::scene::meminfo.materials).c_str());
        ImGuiUtil::TextFocused("Meshes: ", STR_CAST(scene->mNumMeshes).c_str());
        ImGui::SameLine();
        ImGuiUtil::TextFocused("Memory: ", STR_CAST(asset::scene::meminfo.meshes).c_str());
        ImGuiUtil::TextFocused("Skeletons: ", STR_CAST(scene->mNumSkeletons).c_str());
        ImGuiUtil::TextFocused("Textures: ", STR_CAST(scene->mNumTextures).c_str());
        ImGui::SameLine();
        ImGuiUtil::TextFocused("Memory: ", STR_CAST(asset::scene::meminfo.textures).c_str());

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (scene->mNumAnimations > 0 && ImGui::TreeNode("Animation Data")) {
            static int selected_anim = 0;
            const auto num_anims = static_cast<int>(scene->mNumAnimations);
            selected_anim = std::clamp(selected_anim, 0, num_anims - 1);
            ImGui::SliderInt("Animation", &selected_anim, 0, num_anims - 1, "%d",
                             ImGuiSliderFlags_NoSpeedTweaks | ImGuiSliderFlags_AlwaysClamp);
            const auto anim = scene->mAnimations[selected_anim];

            ImGuiUtil::TextFocused("Name: ", anim->mName.C_Str());
            ImGuiUtil::TextFocused("Duration (Ticks): ", STR_CAST(anim->mDuration).c_str());
            ImGuiUtil::TextFocused("Ticks Per Second: ", STR_CAST(anim->mTicksPerSecond).c_str());

            static int selected_bone_channel = 0;
            const auto num_bone_chnls = static_cast<int>(anim->mNumChannels);
            selected_bone_channel = std::clamp(selected_bone_channel, 0, num_bone_chnls - 1);
            ImGui::SliderInt("Bone Channel", &selected_bone_channel, 0, num_bone_chnls - 1, "%d",
                             ImGuiSliderFlags_NoSpeedTweaks | ImGuiSliderFlags_AlwaysClamp);
            const auto bone_channel = anim->mChannels[selected_bone_channel];

            ImGuiUtil::TextFocused("Node Name: ", bone_channel->mNodeName.C_Str());
            ImGuiUtil::TextFocused("PreState: ", behaviour_string(bone_channel->mPreState));
            ImGuiUtil::TextFocused("PostState: ", behaviour_string(bone_channel->mPostState));
            ImGuiUtil::TextFocused("Position Keys: ", STR_CAST(bone_channel->mNumPositionKeys).c_str());
            ImGuiUtil::TextFocused("Rotation Keys: ", STR_CAST(bone_channel->mNumRotationKeys).c_str());
            ImGuiUtil::TextFocused("Scaling Keys: ", STR_CAST(bone_channel->mNumScalingKeys).c_str());

            ImGui::TreePop();
        }

        if (scene->mNumCameras > 0 && ImGui::TreeNode("Camera Data")) {
            static int selected_cam = 0;
            const auto num_cams = static_cast<int>(scene->mNumCameras);
            selected_cam = std::clamp(selected_cam, 0, num_cams - 1);
            ImGui::SliderInt("Camera", &selected_cam, 0, num_cams - 1, "%d",
                             ImGuiSliderFlags_NoSpeedTweaks | ImGuiSliderFlags_AlwaysClamp);

            const auto cam = scene->mCameras[selected_cam];
            ImGuiUtil::TextFocused("Name: ", cam->mName.C_Str());
            ImGuiUtil::TextFocused("Position: ", VEC_STR(cam->mPosition).c_str());
            ImGuiUtil::TextFocused("mUp: ", VEC_STR(cam->mUp).c_str());
            ImGuiUtil::TextFocused("mLookAt: ", VEC_STR(cam->mLookAt).c_str());
            ImGuiUtil::TextFocused("Aspect Ratio: ", STR_CAST(cam->mAspect).c_str());
            ImGuiUtil::TextFocused("Orthographic Width: ", STR_CAST(cam->mOrthographicWidth).c_str());
            ImGuiUtil::TextFocused("Horizontal FOV (radians): ", STR_CAST(cam->mHorizontalFOV).c_str());
            ImGuiUtil::TextFocused("Clip Plane (near): ", STR_CAST(cam->mClipPlaneNear).c_str());
            ImGuiUtil::TextFocused("Clip Plane (far): ", STR_CAST(cam->mClipPlaneFar).c_str());

            ImGui::TreePop();
        }

        ImGui::End();
    }

    void converter_window() {
        ImGui::Begin("X Asset Converter", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        static int export_fmt = 0;
        const auto preview_format = asset::EXPORTER_FORMAT_PREVIEW[export_fmt];
        if (ImGui::BeginCombo("Exported Format", preview_format, ImGuiComboFlags_WidthFitPreview)) {
            for (int i = 0; i < IM_ARRAYSIZE(asset::EXPORTER_FORMAT_PREVIEW); i++) {
                const bool is_selected = export_fmt == i;
                if (ImGui::Selectable(asset::EXPORTER_FORMAT_PREVIEW[i], is_selected)) export_fmt = i;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected) ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        static char asset_file[128] = "test.X";
        ImGui::InputText("Asset Filename", asset_file, IM_ARRAYSIZE(asset_file));

        if (ImGui::Button("Fix")) {
            const auto asset_path = fs::path(asset_file);
            asset::fix(asset_path.is_absolute() ? asset_path : in_assets_dir / asset_file, out_fixed_dir / asset_file);
        }

        ImGui::SameLine();

        if (ImGui::Button("Fix All")) {
            for (const auto &entry: fs::recursive_directory_iterator(in_assets_dir)) {
                if (!entry.is_regular_file()) continue;
                const auto &entry_path = entry.path();
                const auto out = out_fixed_dir / relative(entry_path, in_assets_dir);

                if (!asset::fix(entry_path, out)) {
                    spdlog::error("❌ Failed to fix all assets. Stopped to prevent issues.");
                    break;
                }
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Load")) {
            const auto asset_path = fs::path(asset_file);
            asset::load(asset_path.is_absolute() ? asset_path : out_fixed_dir / asset_file);
        }

        ImGui::SameLine();

        ImGui::BeginDisabled(asset::scene::instance == nullptr);
        if (ImGui::Button("Dump Loaded")) {
            auto asset_path = fs::path(asset_file);

            const auto name = asset_path.stem().string().append("_dump");
            const auto ext = asset::FILE_FORMATS[export_fmt];

            asset::dump(asset::EXPORTER_FORMATS[export_fmt],
                        asset_path.is_absolute()
                            ? asset_path.replace_filename(name).replace_extension(ext)
                            : (out_assets_dir / asset_file).replace_extension(ext));
        }
        ImGui::EndDisabled();

        ImGui::SameLine();

        if (ImGui::Button("Dump Normal")) {
            for (const auto &entry: fs::recursive_directory_iterator(in_assets_dir)) {
                if (!entry.is_regular_file()) continue;
                const auto &entry_path = entry.path();
                const auto out_path = fs::path(entry_path).replace_extension(asset::FILE_FORMATS[export_fmt]);

                if (!asset::load(entry_path) || !asset::dump(asset::EXPORTER_FORMATS[export_fmt], out_path)) {
                    spdlog::error("❌ Failed to dump all normal assets. Stopped to prevent issues.");
                    break;
                }
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Dump Fixed")) {
            for (const auto &entry: fs::recursive_directory_iterator(out_fixed_dir)) {
                if (!entry.is_regular_file()) continue;
                const auto &entry_path = entry.path();
                const auto out_path = (out_assets_dir / relative(entry_path, out_fixed_dir)).replace_extension(
                    asset::FILE_FORMATS[export_fmt]);

                if (!asset::load(entry_path) || !asset::dump(asset::EXPORTER_FORMATS[export_fmt], out_path)) {
                    spdlog::error("❌ Failed to dump all fixed assets. Stopped to prevent issues.");
                    break;
                }
            }
        }

        ImGui::End();
    }

    void render() {
        converter_window();
        scene_window();
    }
}
