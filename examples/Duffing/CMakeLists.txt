cmake_minimum_required(VERSION 3.14)
project(phonos LANGUAGES CXX)

include("../../cmake/utils.cmake")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    find_package(phonos REQUIRED)
endif()

set(sources main.cpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

add_executable(phonos-Duffing)
target_sources(phonos-Duffing PRIVATE ${sources})
target_link_libraries(phonos-Duffing PRIVATE phonos::phonos)

if(NOT is_top_level)
    win_copy_deps_to_target_dir(phonos-Duffing phonos::phonos)
endif()