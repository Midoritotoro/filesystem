cmake_minimum_required(VERSION 4.1  FATAL_ERROR)

project(filesystemTestsTypeTraits LANGUAGES CXX)


set(filesystem_DIR "${CMAKE_CURRENT_LIST_DIR}/../../out/build")


if (PROJECT_IS_TOP_LEVEL)
    find_package(filesystem REQUIRED COMPONENTS
        filesystem 
   )
endif()

enable_testing()
macro(filesystem_io_add_test filename)
    add_executable(${filename} ${filename}.cpp)
    add_compile_options(/bigobj)

    target_link_libraries(
      ${filename} PRIVATE
      filesystem::filesystem
    )

    add_test(${filename} ${CMAKE_CURRENT_BINARY_DIR}/"io"/${filename})
    target_compile_features(${filename} PUBLIC cxx_std_23)
endmacro(filesystem_io_add_test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB ioTests *.cpp)

FOREACH(testsource ${ioTests})
    GET_FILENAME_COMPONENT(filename ${testsource} NAME_WE)
    filesystem_io_add_test(${filename})
ENDFOREACH()