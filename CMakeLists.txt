cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

project(flo CXX)

enable_testing()

add_subdirectory("testing")
add_subdirectory("libflo")
