cmake_minimum_required(VERSION 3.5)
project(ColorCluster)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(C:\\\\opencv\\\\mingw\\\\install\\\\include)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
#SET(PROJECT_BINARY_DIR "E:\\\\My Project\\\\OpenCV\\\\\RamonORC")
set(SOURCE_FILES main.cpp)
set(TARGET_LIBS
        "C:\\\\opencv\\\\mingw\\\\install\\\\x86\\\\mingw\\\\lib\\\\libopencv_world310.dll.a"
        "C:\\\\opencv\\\\mingw\\\\install\\\\x86\\\\mingw\\\\lib\\\\libopencv_ts310.a")

add_executable(ColorCluster ${SOURCE_FILES})

target_link_libraries(ColorCluster ${TARGET_LIBS})