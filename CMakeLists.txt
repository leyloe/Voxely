cmake_minimum_required(VERSION 4.2)
project(voxely)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES})

if(WIN32)
    add_compile_definitions(WIN32 _WINDOWS)
endif()

add_subdirectory(extern/Donut)

target_link_libraries(${PROJECT_NAME} donut_app)

target_include_directories(${PROJECT_NAME} PRIVATE src)

include(extern/Donut/compileshaders.cmake)

donut_compile_shaders_all_platforms(
        TARGET ${PROJECT_NAME}_shaders
        CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders.cfg
        SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders.hlsl
        FOLDER ${PROJECT_NAME}
        OUTPUT_BASE ${CMAKE_BINARY_DIR}/shaders/${PROJECT_NAME}
)

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_shaders)

# Copy runtime assets (textures) next to the executable so they resolve at runtime.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/textures
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/textures)