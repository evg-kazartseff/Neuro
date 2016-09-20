cmake_minimum_required(VERSION 3.0.0)
project(neuro)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall -g -O0)
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)

add_subdirectory(src)
