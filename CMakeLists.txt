cmake_minimum_required(VERSION 3.20)

project(RedClone)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(RedClone ${SOURCES})

target_include_directories(RedClone PRIVATE src)

add_subdirectory(tests)
