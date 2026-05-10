include(CMakeFindDependencyMacro)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/filesystemTargets.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/filesystemTargets.cmake")
endif()