cmake_minimum_required(VERSION 3.13)
project(SYCLKernels
        VERSION 0.1.0
        HOMEPAGE_URL https://github.com/alex-dev/NativeVoxelizer/SYCLKernels
        LANGUAGES CXX)

set(SYCL_SOURCES
    src/impl/debug_controller.cpp
    src/impl/meshes_controller.cpp
    src/impl/vertices_converters.cpp)

add_library(SYCLKernels STATIC
            ${SYCL_SOURCES}
            src/debug_controller.cpp
            src/master_controller.cpp
            src/meshes_controller.cpp
            src/impl/logger.cpp
            src/impl/master_controller.cpp)
target_include_directories(SYCLKernels
                           PUBLIC include)
target_include_directories(SYCLKernels SYSTEM
                           PRIVATE ${OpenCL_INCLUDE_DIRS} ${ComputeCpp_INCLUDE_DIRS})
target_link_libraries(SYCLKernels
                      PUBLIC External)

add_sycl_to_target(TARGET SYCLKernels
                   SOURCES ${SYCL_SOURCES})
