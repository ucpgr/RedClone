cmake_minimum_required(VERSION 3.20)
project(RedClone)
include(FetchContent)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

FetchContent_Declare(sfml GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 3.1.0 GIT_SHALLOW ON)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(sfml)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
FetchContent_Declare(
    mathfu
    GIT_REPOSITORY https://github.com/google/mathfu.git
    GIT_TAG master
    GIT_SUBMODULES_RECURSE TRUE
)

FetchContent_GetProperties(mathfu)
if(NOT mathfu_POPULATED)
    FetchContent_Populate(mathfu)
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(RedClone ${SOURCES})
target_include_directories(RedClone PRIVATE
    src 
    ${mathfu_SOURCE_DIR}/include
    ${mathfu_SOURCE_DIR}/dependencies/vectorial/include
)
target_link_libraries(RedClone PRIVATE SFML::Graphics SFML::Window SFML::System)
add_subdirectory(tests)
