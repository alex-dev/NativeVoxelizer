cmake_minimum_required(VERSION 3.13)
project(NativeVoxelizer
        VERSION 0.1.0
        HOMEPAGE_URL https://github.com/alex-dev/NativeVoxelizer)

option(NATIVE_VOXELIZER_USE_CXX_CLI "Is C++/CLI available." OFF)

if(MSVC)
  string(REGEX REPLACE "/std:c\\+\\+[0-9]*" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  string(REGEX REPLACE "/W[a-zA-Z0-9]*" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  string(PREPEND CMAKE_CXX_FLAGS
      "/permissive- /Zc:__cplusplus /W4 /WX /experimental:external /external:W0 /external:templates- ")
  set(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "/external:I ")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(TO_CMAKE_PATH $ENV{COMPUTECPP_ROOT_DIR}/cmake ENV_COMPUTECPP_CMAKE_DIR)
list(APPEND CMAKE_MODULE_PATH ${ENV_COMPUTECPP_CMAKE_DIR})
include(FindComputeCpp)
include(ComputeCppCompilerChecks)

message(STATUS "OpenCL include directory: ${OpenCL_INCLUDE_DIRS}")
message(STATUS "ComputeCpp include directory: ${ComputeCpp_INCLUDE_DIRS}")

add_subdirectory(External)
add_subdirectory(SYCLKernels)

if(NOT NATIVE_VOXELIZER_USE_CXX_CLI)
  add_subdirectory(ExternCInterface)
endif()
