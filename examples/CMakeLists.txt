cmake_minimum_required(VERSION 3.30)
project(phonos_examples LANGUAGES CXX)

include("../cmake/utils.cmake")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    find_package(phonos REQUIRED)
endif()

# Define all example executables
set(examples
    duffing_example
    single_reed_example
    voice_example
    webster_fdtd_example
    webster_fdtd_timing_example
    benchmark_biquad_example
)

# Add read/write examples only if HDF5 is enabled
if(PHONOS_USE_HDF5)
    list(APPEND examples
        voice_read_write_example
        voice_read_write_chirp_example
        webster_fdtd_read_write_example
    )
endif()

# Create each example executable
foreach(example IN LISTS examples)
    # Get the source file name (example with .cpp extension)
    set(source_file "${example}.cpp")
    
    add_executable(phonos-${example})
    target_sources(phonos-${example} PRIVATE ${source_file})
    target_link_libraries(phonos-${example} PRIVATE phonos::phonos)
    
    if(NOT is_top_level)
        win_copy_deps_to_target_dir(phonos-${example} phonos::phonos)
    endif()
endforeach()

# If you want to build all examples by default, uncomment the next line:
# add_custom_target(all_examples DEPENDS ${examples})