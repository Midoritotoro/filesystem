cmake_minimum_required (VERSION 4.1  FATAL_ERROR)

project ("filesystem"
    VERSION 1.0.0
    DESCRIPTION "..."
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_VERBOSE_MAKEFILE ON)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()
if (NOT DEFINED CMAKE_INSTALL_BINDIR)
  set(CMAKE_INSTALL_BINDIR "bin")
endif()
if (NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
  set(CMAKE_INSTALL_INCLUDEDIR "include")
endif()

option(FILESYSTEM_INSTALL "" ${PROJECT_IS_TOP_LEVEL})


set(FILESYSTEM_INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/filesystem")
file(GLOB_RECURSE PRIVATE_SOURCES CONFIGURE_DEPENDS "src/filesystem/*.cpp" "src/filesystem/*.c")
file(GLOB_RECURSE PRIVATE_HEADERS CONFIGURE_DEPENDS "src/filesystem/*.h" "src/filesystem/*.hpp" "src/filesystem/*.inl")
file(GLOB_RECURSE PUBLIC_HEADERS CONFIGURE_DEPENDS "include/filesystem/*.h" "include/filesystem/*.hpp" "include/filesystem/*.inl")

include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

add_library(filesystem STATIC ${PRIVATE_SOURCES} ${PRIVATE_HEADERS} ${PUBLIC_HEADERS})
add_library(filesystem::filesystem ALIAS filesystem)

target_sources(filesystem PRIVATE ${PRIVATE_SOURCES} ${PRIVATE_HEADERS})

target_include_directories(filesystem PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

set_target_properties(
    filesystem PROPERTIES
    
    CXX_STANDARD_REQUIRED ON
    CXX_STANDARD 23
    CXX_EXTENSIONS OFF

    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"

    EXPORT_NAME filesystem
    OUTPUT_NAME filesystem

    LINKER_LANGUAGE CXX
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_features(filesystem PUBLIC cxx_std_23)
    set(CMAKE_DEBUG_TARGET_PROPERTIES "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g) 
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O2 -GL- -Ob3)
    endif()

    target_link_options(filesystem PRIVATE "/LTCG;/opt:ref,icf;")

    target_compile_options(
        filesystem 
        PRIVATE
            /bigobj
            /permissive-
            /W4
            
            /utf-8

            /MP
            /EHsc

            /guard:cf

            /w15038 
            /w14265 
            /wd4018
            /wd4100
            /wd4242
            /wd4244
            /wd4245
            /wd4267
            /wd4305
            /wd4324
            /wd4389
            /wd4456
            /wd4457
            /wd4458
            /wd4459
            /wd4611
            /wd4702
            /wd4828

            /Zi
            /Zc:__cplusplus
    )   
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    
    target_compile_options(
        filesystem 
        PRIVATE
            /permissive-
            /W4
            /utf-8
            /MP
            /O3
            /EHsc
            /Zi
            /Zc:__cplusplus
        )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(
        filesystem 
        PRIVATE
            /utf-8

            /O3
            /W4

            /EHsc   

            /Zi
            /Zc:__cplusplus
    )
endif()

if(PROJECT_IS_TOP_LEVEL)
    enable_testing()
    add_subdirectory(tests)
endif()

if(FILESYSTEM_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    include(cmake/Util_InstallRules.cmake)
endif()
