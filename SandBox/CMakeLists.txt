cmake_minimum_required(VERSION 3.5)
project(SandBox)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 3)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_subdirectory(../Engine Subdirectory/Engine/)
add_subdirectory(../Modules Subdirectory/Modules/)

file(GLOB SANDBOX_SOURCE_FILES
        "Source/*.cpp"
)

add_executable(SandBox ${SANDBOX_SOURCE_FILES})
target_link_libraries(SandBox PUBLIC Oneiro-Common Oneiro-Core Oneiro-Module-WM-SDL Oneiro-Module-RHI-GL)
file(COPY "${CMAKE_CURRENT_BINARY_DIR}/Subdirectory/Engine/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")