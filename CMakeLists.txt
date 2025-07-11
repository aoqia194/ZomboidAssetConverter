cmake_minimum_required(VERSION 3.30)

project(ZomboidAssetConverter LANGUAGES CXX VERSION "1.1.1")
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SPDLOG_BUILD_EXAMPLE OFF)

find_package(assimp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(cxxopts CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)

# For assimp to be built as a static library.
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(src)
