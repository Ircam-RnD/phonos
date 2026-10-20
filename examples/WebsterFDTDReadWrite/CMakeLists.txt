cmake_minimum_required(VERSION 3.30)
project(phonos LANGUAGES CXX)

if(TARTE_USE_HDF5)
    include("../../cmake/utils.cmake")
    string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

    if(is_top_level)
        find_package(phonos REQUIRED)
    endif()

    set(sources main.cpp)
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

    add_executable(phonos-websterFDTDReadWrite)
    target_sources(phonos-websterFDTDReadWrite PRIVATE ${sources})
    target_link_libraries(phonos-websterFDTDReadWrite PRIVATE phonos::phonos)

    if(NOT is_top_level)
        win_copy_deps_to_target_dir(phonos-websterFDTDReadWrite phonos::phonos)
    endif()
endif()