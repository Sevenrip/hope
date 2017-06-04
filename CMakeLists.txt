cmake_minimum_required (VERSION 3.6)
project (hope)
# The version number.
set (hope_VERSION_MAJOR 0)
set (hope_VERSION_MINOR 1)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCES      src/*.cpp)

include_directories("${PROJECT_SOURCE_DIR}/include")
 
# add the executable
add_executable(hope ${SOURCES})

target_compile_options(hope PUBLIC -std=c++14 -Wall -Werror)

