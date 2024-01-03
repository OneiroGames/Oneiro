cmake_minimum_required(VERSION 3.5)
project(WM-SDL)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 3)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

file(GLOB ONEIRO_MODULE_WM_SDL_SOURCE_FILES
        "Source/*.cpp"
)

add_library(Oneiro-Module-WM-SDL SHARED ${ONEIRO_MODULE_WM_SDL_SOURCE_FILES})
target_link_libraries(Oneiro-Module-WM-SDL PUBLIC Oneiro-Common $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static> imgui::imgui)
target_include_directories(Oneiro-Module-WM-SDL PUBLIC "Include/")
set_target_properties(Oneiro-Module-WM-SDL
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Modules/Oneiro-Module-WM-SDL/"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Modules/Oneiro-Module-WM-SDL/"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Modules/Oneiro-Module-WM-SDL/"
)
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/module.json" DESTINATION "${CMAKE_BINARY_DIR}/Modules/Oneiro-Module-WM-SDL/")