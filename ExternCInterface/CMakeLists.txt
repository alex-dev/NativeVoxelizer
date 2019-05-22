cmake_minimum_required(VERSION 3.13)
project(ExternCInterface
        VERSION 0.1.0
        HOMEPAGE_URL https://github.com/alex-dev/NativeVoxelizer/ExternCInterface
        LANGUAGES CXX)

add_library(ExternCInterface SHARED
            src/debug.cpp
            src/master.cpp
            src/meshes.cpp)
target_include_directories(ExternCInterface
                           PUBLIC include)
target_link_libraries(ExternCInterface
                      PRIVATE SYCLKernels External)
