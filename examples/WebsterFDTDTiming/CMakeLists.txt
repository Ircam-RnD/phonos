cmake_minimum_required(VERSION 3.30)
project(phonos LANGUAGES CXX)

include("../../cmake/utils.cmake")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    find_package(phonos REQUIRED)
endif()

set(sources main.cpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

add_executable(phonos-websterFDTDTiming)
target_sources(phonos-websterFDTDTiming PRIVATE ${sources})
target_link_libraries(phonos-websterFDTDTiming PRIVATE phonos::phonos)

if(NOT is_top_level)
    win_copy_deps_to_target_dir(phonos-websterFDTDTiming phonos::phonos)
endif()