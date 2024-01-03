cmake_minimum_required(VERSION 3.5)
project(Oneiro-Engine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 3)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

cmake_policy(SET CMP0079 NEW)

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

add_library(Oneiro-Common STATIC ${ONEIRO_COMMON_SOURCE_FILES})
target_include_directories(Oneiro-Common PUBLIC "Include/")
target_link_libraries(Oneiro-Common PUBLIC fmt::fmt glm::glm PhysFS::PhysFS spdlog::spdlog nameof::nameof xxHash::xxhash
        flecs::flecs rapidjson)
target_precompile_headers(Oneiro-Common PUBLIC ${ONEIRO_COMMON_PCH_FILES})

file(GLOB ONEIRO_RENDERING_SOURCE_FILES
        "Source/Rendering/*.cpp"
        "Source/Rendering/ImGui/*.cpp"
)

add_library(Oneiro-Rendering SHARED ${ONEIRO_RENDERING_SOURCE_FILES})
target_include_directories(Oneiro-Rendering PUBLIC "Include/")
target_link_libraries(Oneiro-Rendering PUBLIC Oneiro-Common imgui::imgui imguizmo::imguizmo)
target_precompile_headers(Oneiro-Rendering PUBLIC ${ONEIRO_COMMON_PCH_FILES})

file(GLOB ONEIRO_CORE_SOURCE_FILES
        "Source/Core/*.cpp"
)

add_library(Oneiro-Core SHARED ${ONEIRO_CORE_SOURCE_FILES})
target_link_libraries(Oneiro-Core PUBLIC Oneiro-Common Oneiro-Rendering)