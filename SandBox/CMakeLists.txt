cmake_minimum_required(VERSION 3.5)
project(SandBox)

file(GLOB SANDBOX_SOURCE_FILES
        "Source/*.cpp"
)

add_executable(SandBox ${SANDBOX_SOURCE_FILES})
target_link_libraries(SandBox PUBLIC Oneiro-Common Oneiro-Core Oneiro-Module-WM-SDL Oneiro-Module-RHI-GL)
set_target_properties(SandBox
        PROPERTIES
        CXX_STANDARD 23
        UNITY_BUILD ON
        UNITY_BUILD_BATCH_SIZE 3
        WINDOWS_EXPORT_ALL_SYMBOLS ON

        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
)