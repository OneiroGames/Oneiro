cmake_minimum_required(VERSION 3.7.2)
project(Oneiro)

set(CMAKE_CXX_STANDARD 23)

if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

if (MSVC)
    # nothing special for MSVC at the moment
else ()
    add_compile_options("-Wall" "-Wextra" "-Werror" "$<$<CONFIG:RELEASE>:-O3>")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options("-stdlib=libc++")
    else ()
        # nothing special for gcc at the moment
    endif ()
endif ()

find_package(SDL2 CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_path(SIMPLEINI_INCLUDE_DIRS "ConvertUTF.c")
find_package(physfs CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(nameof CONFIG REQUIRED)
find_package(Tracy CONFIG REQUIRED)

set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 3)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

file(GLOB ONEIRO_COMMON_PCH_FILES
        "Engine/Source/Common/Common.hpp"
)

file(GLOB ONEIRO_COMMON_SOURCE_FILES
        "Engine/Source/Common/*.cpp"
        "Engine/Source/Common/Signals/*.cpp"
        "Engine/Source/Common/FileSystem/*.cpp"
        "Engine/Source/Common/Math/*.cpp"
        "Engine/Source/Common/WM/*.cpp"
)

add_library(Oneiro-Common STATIC ${ONEIRO_COMMON_SOURCE_FILES})
target_include_directories(Oneiro-Common PUBLIC "Engine/Include/" ${SIMPLEINI_INCLUDE_DIRS})
target_link_libraries(Oneiro-Common PUBLIC yaml-cpp fmt::fmt glm::glm PhysFS::PhysFS spdlog::spdlog nameof::nameof Tracy::TracyClient)
target_precompile_headers(Oneiro-Common PUBLIC ${ONEIRO_COMMON_PCH_FILES})

file(GLOB ONEIRO_MODULE_WM_SDL_SOURCE_FILES
        "Engine/Source/Modules/WM/*.cpp"
)

add_library(Oneiro-Module-WM-SDL SHARED ${ONEIRO_MODULE_WM_SDL_SOURCE_FILES})
target_link_libraries(Oneiro-Module-WM-SDL PUBLIC Oneiro-Common $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static> imgui::imgui)

file(GLOB ONEIRO_RENDERER_SOURCE_FILES
        "Engine/Source/Renderer/*.cpp"
)

add_library(Oneiro-Renderer SHARED ${ONEIRO_RENDERER_SOURCE_FILES})
target_link_libraries(Oneiro-Renderer PUBLIC Oneiro-Common imgui::imgui imguizmo::imguizmo)

file(GLOB ONEIRO_CORE_SOURCE_FILES
        "Engine/Source/Core/*.cpp"
)

add_library(Oneiro-Core SHARED ${ONEIRO_CORE_SOURCE_FILES})
target_link_libraries(Oneiro-Core PUBLIC Oneiro-Common)

file(GLOB SANDBOX_SOURCE_FILES
        "SandBox/Source/*.cpp"
)

add_executable(SandBox ${SANDBOX_SOURCE_FILES})
target_link_libraries(SandBox PUBLIC Oneiro-Core Oneiro-Module-WM-SDL)