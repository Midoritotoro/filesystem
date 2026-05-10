cmake_minimum_required(VERSION 4.1  FATAL_ERROR)

project(filesystemTests LANGUAGES CXX)

set(filesystem_DIR "C:/Users/danya/source/repos/Midoritotoro/filesystem/out/build")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(filesystem)

add_subdirectory(sync)