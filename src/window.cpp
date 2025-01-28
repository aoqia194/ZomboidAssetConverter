#define GL_SILENCE_DEPRECATION

#include "window.h"

#include "font.cpp"
#include "gui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "main.h"

#include "imgui_freetype.h"

#include "glad\glad.h"

#include "GLFW\glfw3.h"

#include "spdlog\spdlog.h"

namespace window {
    inline std::shared_ptr<GLFWwindow *> window;

    static void glfw_error_callback(int error, const char *description) {
        spdlog::error("GLFW Error {}: {}", error, description);
    }

    void cleanup() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(*window);
        glfwTerminate();
    }

    int init() {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit()) return 1;

        // Decide GL+GLSL versions
#if defined(__APPLE__)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        window = std::make_shared<GLFWwindow *>(
            glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, PROJECT_NAME " [DEBUG]", nullptr, nullptr));
        if (window == nullptr) {
            spdlog::error("Failed to create GLFW window");
            glfwTerminate();
            return 1;
        };

        glfwMakeContextCurrent(*window);
        if (gladLoadGL() == 0) {
            printf("Failed to initialize OpenGL context\n");
            return -1;
        }
        glfwSwapInterval(1);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(*window, true);
        ImGui_ImplOpenGL3_Init();

        ImFontConfig cfg;
        cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint;
        IM_ASSERT(io.Fonts->AddFontFromMemoryCompressedTTF(opensans_compressed_data,
                opensans_compressed_size,
                16.0f,
                &cfg,
                io.Fonts->GetGlyphRangesDefault())
            != nullptr);

        while (!glfwWindowShouldClose(*window)) {
            glfwPollEvents();

            if (glfwGetWindowAttrib(*window, GLFW_ICONIFIED) != 0) {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            gui::render();
            ImGui::Render();

            int display_w, display_h;
            glfwGetFramebufferSize(*window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(*window);
        }

        cleanup();
        return 0;
    }
}
