cmake_minimum_required(VERSION 3.13)
project(External
        VERSION 0.1.0
        HOMEPAGE_URL https://github.com/alex-dev/NativeVoxelizer/External
        LANGUAGES CXX)

add_library(External OBJECT src/guid.cpp)
target_include_directories(External PUBLIC include)
