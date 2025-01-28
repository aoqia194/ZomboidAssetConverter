set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${SRC_DIR}/*.h")
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")

if (MSVC)
    add_compile_options(
            $<$<CONFIG:>:/MT>
            $<$<CONFIG:Debug>:/MTd>
            $<$<CONFIG:Release>:/MT>
    )
endif ()

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23)

if (CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Werror -Wno-unused-)
elseif (MSVC)
    # DONT USE MSVC, IT'S CANCER!!!!
    target_compile_options(${PROJECT_NAME} PRIVATE /Wall)
endif ()

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        Boost::lexical_cast
        spdlog::spdlog
        cxxopts::cxxopts
        glad::glad
        glfw
        Freetype::Freetype
        imgui::imgui
        assimp::assimp)

if (MSVC)
    set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_DEBUGGER_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/$<TARGET_FILE_NAME:${CMAKE_PROJECT_NAME}>)
    set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)
endif ()
