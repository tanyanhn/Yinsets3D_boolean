cmake_minimum_required(VERSION 3.20)
project(YSB3D)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
# set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -ggdb  -D_intersectTest")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.76.0 REQUIRED timer)

enable_testing()
include_directories(src)
add_subdirectory(test)
