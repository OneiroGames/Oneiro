cmake_minimum_required(VERSION 3.5)
project(glad)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 3)

find_package(OpenGL REQUIRED)

add_library(glad STATIC "src/gl.c")
target_include_directories(glad PUBLIC include)