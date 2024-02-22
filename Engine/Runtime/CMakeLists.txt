cmake_minimum_required(VERSION 3.5)
project(Oneiro-Engine)

cmake_policy(SET CMP0079 NEW)
set(SDW_BUILD_STATIC BOOL ON)

find_package(glm CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)
find_package(physfs CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(nameof CONFIG REQUIRED)
find_package(xxHash CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)

add_subdirectory(./ThirdParty/ShaderWriter)
target_compile_definitions(ShaderWriter PUBLIC SDW_EnableStructHelper=1)

file(GLOB ONEIRO_COMMON_PCH_FILES
        "Include/Common/Common.hpp"
        "Include/Common/EngineApi.hpp"
)

file(GLOB ONEIRO_COMMON_SOURCE_FILES
        "Source/Common/*.cpp"
        "Source/Common/FileSystem/*.cpp"
        "Source/Common/RHI/*.cpp"
        "Source/Common/Math/*.cpp"
        "Source/Common/WM/*.cpp"
        "Source/Common/World/*.cpp"
        "Source/Common/Assets/*.cpp"
)

add_library(Oneiro-Common SHARED ${ONEIRO_COMMON_SOURCE_FILES})
target_include_directories(Oneiro-Common PUBLIC "Include/")
target_link_libraries(Oneiro-Common PUBLIC fmt::fmt glm::glm PhysFS::PhysFS spdlog::spdlog nameof::nameof xxHash::xxhash
        flecs::flecs rapidjson)
target_precompile_headers(Oneiro-Common PUBLIC ${ONEIRO_COMMON_PCH_FILES})
set_target_properties(Oneiro-Common
        PROPERTIES
        CXX_STANDARD 23
        UNITY_BUILD ON
        UNITY_BUILD_BATCH_SIZE 3
        WINDOWS_EXPORT_ALL_SYMBOLS ON

        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
)

file(GLOB ONEIRO_RENDERING_SOURCE_FILES
        "Source/Rendering/*.cpp"
        "Source/Rendering/ImGui/*.cpp"
)

add_library(Oneiro-Rendering SHARED ${ONEIRO_RENDERING_SOURCE_FILES})
target_include_directories(Oneiro-Rendering PUBLIC "Include/")
target_link_libraries(Oneiro-Rendering PUBLIC Oneiro-Common imgui::imgui imguizmo::imguizmo sdw::ShaderWriter sdw::ShaderAST sdw::GlslCommon sdw::CompilerGlsl sdw::CompilerHlsl)
set_target_properties(Oneiro-Rendering
        PROPERTIES
        CXX_STANDARD 23
        UNITY_BUILD ON
        UNITY_BUILD_BATCH_SIZE 3
        WINDOWS_EXPORT_ALL_SYMBOLS ON

        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
)

file(GLOB ONEIRO_CORE_SOURCE_FILES
        "Source/Core/*.cpp"
)

add_library(Oneiro-Core SHARED ${ONEIRO_CORE_SOURCE_FILES})
target_link_libraries(Oneiro-Core PUBLIC Oneiro-Common Oneiro-Rendering)
set_target_properties(Oneiro-Core
        PROPERTIES
        CXX_STANDARD 23
        UNITY_BUILD ON
        UNITY_BUILD_BATCH_SIZE 3
        WINDOWS_EXPORT_ALL_SYMBOLS ON

        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    file(COPY "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin/" DESTINATION "${CMAKE_BINARY_DIR}/")
else ()
    file(COPY "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/" DESTINATION "${CMAKE_BINARY_DIR}/")
endif ()