cmake_minimum_required(VERSION 3.5)
project(RamonORC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(C:\\\\opencv\\\\mingw\\\\install\\\\include)

set(TARGET_LIBS
        "C:\\\\opencv\\\\mingw\\\\install\\\\x86\\\\mingw\\\\lib\\\\libopencv_world310.dll.a"
        "C:\\\\opencv\\\\mingw\\\\install\\\\x86\\\\mingw\\\\lib\\\\libopencv_ts310.a")

set(SOURCE_FILES main.cpp)
add_executable(RamonORC ${SOURCE_FILES})