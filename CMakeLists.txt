cmake_minimum_required(VERSION 3.5)
project(Pointer LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_options(-static)
add_link_options(-static)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)

FetchContent_MakeAvailable(SFML)

add_executable(pointer src/main.cpp src/Entities/enemy.cpp) 

add_custom_command(TARGET pointer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)

target_compile_features(pointer PRIVATE cxx_std_17)
target_link_libraries(pointer PRIVATE SFML::Graphics SFML::Audio)