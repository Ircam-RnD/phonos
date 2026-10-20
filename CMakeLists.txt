cmake_minimum_required(VERSION 3.30)
project(phonos
    VERSION 1.0.0
    DESCRIPTION "Physical audio synthesis engine"
    LANGUAGES CXX)

#----------------------------------------------------------------------------------------------------------------------
# general settings and options
#----------------------------------------------------------------------------------------------------------------------

include(cmake/utils.cmake)
include(GNUInstallDirs)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

# Export compile config
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

# TARTE_SHARED_LIBS option (undefined by default) can be used to force shared/static build
option(TARTE_BUILD_TESTS "Build phonos tests" OFF)
option(TARTE_BUILD_EXAMPLES "Build phonos examples" ON)
option(TARTE_INSTALL "Generate target for installing phonos" ${is_top_level})
option(TARTE_USE_HDF5 "Enable HDF5 support" ON)
set_if_undefined(TARTE_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/phonos" CACHE STRING
    "Install path for phonos package-related CMake files")

if(DEFINED TARTE_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${TARTE_SHARED_LIBS})
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)
set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(phonos) # initialized below
add_library(phonos::phonos ALIAS phonos)

#----------------------------------------------------------------------------------------------------------------------
# phonos dependencies
#----------------------------------------------------------------------------------------------------------------------
target_compile_features(phonos PUBLIC cxx_std_23)
# Search for dependencies here
find_package(Eigen3 5.0 REQUIRED NO_MODULE)

#----------------------------------------------------------------------------------------------------------------------
# phonos sources
#----------------------------------------------------------------------------------------------------------------------

include(GenerateExportHeader)
set(export_file_name "export_shared.h")

if(NOT BUILD_SHARED_LIBS)
    set(export_file_name "export_static.h")
endif()

generate_export_header(phonos EXPORT_FILE_NAME include/${export_file_name})

file(GLOB_RECURSE sources
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/external/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/utility/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

#----------------------------------------------------------------------------------------------------------------------
# phonos target
#----------------------------------------------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

target_sources(phonos PRIVATE ${sources})
target_compile_definitions(phonos PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:TARTE_STATIC_DEFINE>")

target_include_directories(phonos
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

set_target_properties(phonos PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION})

target_link_libraries(phonos PUBLIC 
    Eigen3::Eigen)

if(TARTE_USE_HDF5)
    find_package(HDF5 REQUIRED COMPONENTS C CXX)
    target_link_libraries(phonos PUBLIC ${HDF5_LIBRARIES})
    target_compile_definitions(phonos PUBLIC TARTE_HDF5_ENABLED)
endif()

if(TARTE_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    configure_package_config_file(cmake/phonos-config.cmake.in phonos-config.cmake
        INSTALL_DESTINATION "${TARTE_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(phonos-config-version.cmake
        COMPATIBILITY SameMajorVersion)

    install(TARGETS phonos EXPORT tarte_export
        RUNTIME COMPONENT phonos
        LIBRARY COMPONENT phonos NAMELINK_COMPONENT phonos-dev
        ARCHIVE COMPONENT phonos-dev
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(DIRECTORY include/
        TYPE INCLUDE
        COMPONENT phonos-dev)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/phonos/${export_file_name}"
        COMPONENT phonos-dev
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/phonos")

    set(targets_file "phonos-shared-targets.cmake")

    if(NOT BUILD_SHARED_LIBS)
        set(targets_file "phonos-static-targets.cmake")
    endif()

    install(EXPORT tarte_export
        COMPONENT phonos-dev
        FILE "${targets_file}"
        DESTINATION "${TARTE_INSTALL_CMAKEDIR}"
        NAMESPACE phonos::)

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/phonos-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/phonos-config-version.cmake"
        COMPONENT phonos-dev
        DESTINATION "${TARTE_INSTALL_CMAKEDIR}")

    if(MSVC)
        set(pdb_file "")
        set(pdb_file_destination "")

        if(BUILD_SHARED_LIBS)
            set(pdb_file "$<TARGET_PDB_FILE:phonos>")
            set(pdb_file_destination "${CMAKE_INSTALL_BINDIR}")
        else()
            # TARGET_PDB_FILE does not work for pdb file generated for static library build, determining it manually
            set(pdb_file "$<TARGET_FILE_DIR:phonos>/$<TARGET_FILE_PREFIX:phonos>$<TARGET_FILE_BASE_NAME:phonos>.pdb")
            set(pdb_file_destination "${CMAKE_INSTALL_LIBDIR}")
        endif()

        install(FILES "${pdb_file}"
            COMPONENT phonos-dev
            CONFIGURATIONS Debug RelWithDebInfo
            DESTINATION "${pdb_file_destination}"
            OPTIONAL)
    endif()
endif()

#----------------------------------------------------------------------------------------------------------------------
# other targets
#----------------------------------------------------------------------------------------------------------------------

if(TARTE_BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()

if(TARTE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()