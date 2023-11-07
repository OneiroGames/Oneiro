option(FWOG_FSR2_ENABLE "Enable FSR2 for examples that support it (currently 03_gltf_viewer). Windows only!" FALSE)

add_subdirectory(external)

add_custom_target(copy_shaders ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders)
add_custom_target(copy_models ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/models ${CMAKE_CURRENT_BINARY_DIR}/models)
add_custom_target(copy_textures ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/textures ${CMAKE_CURRENT_BINARY_DIR}/textures)

add_executable(01_hello_triangle "01_hello_triangle.cpp" common/Application.cpp common/Application.h vendor/stb_image.cpp)
target_link_libraries(01_hello_triangle PRIVATE glfw lib_glad fwog glm lib_imgui)