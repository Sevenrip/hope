cmake_minimum_required (VERSION 3.6)
project (hope)

# The version number.
set (hope_VERSION_MAJOR 0)
set (hope_VERSION_MINOR 1)

set(PROJECT_BINARY_DIR ../bin)
set(PROJECT_SRC_DIR ../src)
set(PROJECT_INCLUDE_DIR ../include)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

file(GLOB_RECURSE SOURCES ${PROJECT_SRC_DIR}/*.cpp)
 
# add the executable
add_executable(hope ${SOURCES})
target_include_directories(hope PUBLIC ${PROJECT_INCLUDE_DIR})
target_compile_options(hope PUBLIC -std=c++14 -Wall)


