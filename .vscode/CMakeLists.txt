cmake_minimum_required(VERSION 3.26)
project(Oneiro)

set(CMAKE_CXX_STANDARD 23)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

find_package(SDL2 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_path(MINIMP3_INCLUDE_DIRS "minimp3/minimp3.h")
find_package(Vorbis CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(openal CONFIG REQUIRED)
find_package(physfs CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(nameof CONFIG REQUIRED)

set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 3)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

file(GLOB ONEIRO_COMMON_HEADER_FILES
        "Engine/Source/Common/*.hpp"
        "Engine/Source/Common/FileSystem/*.hpp"
        "Engine/Source/Common/WM/*.hpp"
        "Engine/Source/Common/Math/*.hpp"
        "Engine/Source/Common/Events/*.hpp"
)

file(GLOB ONEIRO_COMMON_SOURCE_FILES
        "Engine/Source/Common/*.cpp"
        "Engine/Source/Common/Events/*.cpp"
        "Engine/Source/Common/FileSystem/*.cpp"
        "Engine/Source/Common/Math/*.cpp"
        "Engine/Source/Common/WM/*.cpp"
)

add_library(Oneiro.Common SHARED ${ONEIRO_COMMON_SOURCE_FILES})
target_include_directories(Oneiro.Common PUBLIC "Engine/Source/")
target_link_libraries(Oneiro.Common PUBLIC yaml-cpp fmt::fmt glm::glm PhysFS::PhysFS spdlog::spdlog nameof::nameof)
target_precompile_headers(Oneiro.Common PUBLIC ${ONEIRO_COMMON_HEADER_FILES})

file(GLOB ONEIRO_WM_SDL_HEADER_FILES
        "Engine/Source/WM/SDL/*.hpp"
)

file(GLOB ONEIRO_WM_SDL_SOURCE_FILES
        "Engine/Source/WM/SDL/*.cpp"
)

add_library(Oneiro.WM.SDL SHARED ${ONEIRO_WM_SDL_SOURCE_FILES})
target_link_libraries(Oneiro.WM.SDL PUBLIC Oneiro.Common SDL2::SDL2 glad::glad)
target_precompile_headers(Oneiro.WM.SDL PUBLIC ${ONEIRO_WM_SDL_HEADER_FILES})

file(GLOB ONEIRO_CORE_HEADER_FILES
        "Engine/Source/Core/*.hpp"
)

file(GLOB ONEIRO_CORE_SOURCE_FILES
        "Engine/Source/Core/*.cpp"
)

add_library(Oneiro.Core SHARED ${ONEIRO_CORE_SOURCE_FILES})
target_link_libraries(Oneiro.Core PUBLIC Oneiro.Common)
target_precompile_headers(Oneiro.Core PUBLIC ${ONEIRO_CORE_HEADER_FILES})

file(GLOB ONEIRO_RENDERER_FILES
        "Engine/Source/Renderer/*.hpp"
)

file(GLOB ONEIRO_RENDERER_SOURCE_FILES
        "Engine/Source/Renderer/*.cpp"
)

add_library(Oneiro.Renderer SHARED ${ONEIRO_RENDERER_SOURCE_FILES})
target_link_libraries(Oneiro.Renderer PUBLIC Oneiro.Common)
target_precompile_headers(Oneiro.Renderer PUBLIC ${ONEIRO_RENDERER_HEADER_FILES})

file(GLOB ONEIRO_RENDERER_GL_FILES
        "Engine/Source/Renderer/OpenGL/*.hpp"
)

file(GLOB ONEIRO_RENDERER_GL_SOURCE_FILES
        "Engine/Source/Renderer/OpenGL/*.cpp"
)

add_library(Oneiro.Renderer.GL SHARED ${ONEIRO_RENDERER_GL_SOURCE_FILES})
target_link_libraries(Oneiro.Renderer.GL PUBLIC Oneiro.Common glad::glad)
target_precompile_headers(Oneiro.Renderer.GL PUBLIC ${ONEIRO_RENDERER_GL_HEADER_FILES})

add_executable(SandBox "Engine/Source/Main.cpp")
target_link_libraries(SandBox PUBLIC Oneiro.Core Oneiro.WM.SDL Oneiro.Renderer.GL)
