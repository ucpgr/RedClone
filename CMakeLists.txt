cmake_minimum_required(VERSION 3.20)
project(RedClone)

include(FetchContent)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
enable_testing()

FetchContent_Declare(sfml GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 3.1.0 GIT_SHALLOW ON)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_USE_UDEV OFF CACHE BOOL "" FORCE)
set(SFML_USE_DRM OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(sfml)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
FetchContent_Declare(
    mathfu
    GIT_REPOSITORY https://github.com/google/mathfu.git
    GIT_TAG v1.1.0
    GIT_SUBMODULES_RECURSE TRUE
)

if(POLICY CMP0169)
    cmake_policy(PUSH)
    cmake_policy(SET CMP0169 OLD)
endif()
FetchContent_GetProperties(mathfu)
if(NOT mathfu_POPULATED)
    FetchContent_Populate(mathfu)
endif()
if(POLICY CMP0169)
    cmake_policy(POP)
endif()

FetchContent_Declare(
    lua
    GIT_REPOSITORY https://github.com/lua/lua.git
    GIT_TAG v5.5.0
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(lua)

add_library(lua_static STATIC
    ${lua_SOURCE_DIR}/lapi.c
    ${lua_SOURCE_DIR}/lauxlib.c
    ${lua_SOURCE_DIR}/lbaselib.c
    ${lua_SOURCE_DIR}/lcode.c
    ${lua_SOURCE_DIR}/lcorolib.c
    ${lua_SOURCE_DIR}/lctype.c
    ${lua_SOURCE_DIR}/ldblib.c
    ${lua_SOURCE_DIR}/ldebug.c
    ${lua_SOURCE_DIR}/ldo.c
    ${lua_SOURCE_DIR}/ldump.c
    ${lua_SOURCE_DIR}/lfunc.c
    ${lua_SOURCE_DIR}/lgc.c
    ${lua_SOURCE_DIR}/linit.c
    ${lua_SOURCE_DIR}/liolib.c
    ${lua_SOURCE_DIR}/llex.c
    ${lua_SOURCE_DIR}/lmathlib.c
    ${lua_SOURCE_DIR}/lmem.c
    ${lua_SOURCE_DIR}/loadlib.c
    ${lua_SOURCE_DIR}/lobject.c
    ${lua_SOURCE_DIR}/lopcodes.c
    ${lua_SOURCE_DIR}/loslib.c
    ${lua_SOURCE_DIR}/lparser.c
    ${lua_SOURCE_DIR}/lstate.c
    ${lua_SOURCE_DIR}/lstring.c
    ${lua_SOURCE_DIR}/lstrlib.c
    ${lua_SOURCE_DIR}/ltable.c
    ${lua_SOURCE_DIR}/ltablib.c
    ${lua_SOURCE_DIR}/ltm.c
    ${lua_SOURCE_DIR}/lundump.c
    ${lua_SOURCE_DIR}/lutf8lib.c
    ${lua_SOURCE_DIR}/lvm.c
    ${lua_SOURCE_DIR}/lzio.c
)
add_library(Lua::Lua ALIAS lua_static)
target_include_directories(lua_static PUBLIC ${lua_SOURCE_DIR})
set(LUA_INCLUDE_DIR ${lua_SOURCE_DIR} CACHE PATH "" FORCE)
set(LUA_INCLUDE_DIRS ${lua_SOURCE_DIR} CACHE PATH "" FORCE)
set(LUA_LIBRARIES lua_static CACHE STRING "" FORCE)

FetchContent_Declare(
    luacxx
    GIT_REPOSITORY https://github.com/dafrito/luacxx.git
    GIT_TAG v4.0.0
    GIT_SHALLOW ON
)
if(POLICY CMP0169)
    cmake_policy(PUSH)
    cmake_policy(SET CMP0169 OLD)
endif()
FetchContent_GetProperties(luacxx)
if(NOT luacxx_POPULATED)
    FetchContent_Populate(luacxx)
endif()
if(POLICY CMP0169)
    cmake_policy(POP)
endif()

add_library(luacxx_interface INTERFACE)
add_library(LuaCxx::LuaCxx ALIAS luacxx_interface)
target_include_directories(luacxx_interface INTERFACE ${luacxx_SOURCE_DIR}/include)
target_link_libraries(luacxx_interface INTERFACE Lua::Lua)

target_link_libraries(lua_static PUBLIC m)

set(REDCLONE_SOURCES
    src/core/Application.cpp
    src/core/CameraController.cpp
    src/core/GameLoop.cpp
    src/core/Logger.cpp
    src/ecs/Component.cpp
    src/ecs/EntityManager.cpp
    src/ecs/System.cpp
    src/engine/camera/Camera2D.cpp
    src/engine/input/InputSystem.cpp
    src/engine/input/SfmlInputAdapter.cpp
    src/engine/math/Isometric.cpp
    src/engine/rendering/SfmlRenderer.cpp
    src/engine/window/Window.cpp
    src/gameplay/Order.cpp
    src/gameplay/SelectionController.cpp
    src/main.cpp
    src/scripting/ScriptEngine.cpp
    src/ui/UISystem.cpp
    src/world/Entity.cpp
    src/world/TileMap.cpp
    src/world/TileMapGenerator.cpp
    src/world/TileMapRenderer.cpp
    src/world/TerrainMesh.cpp
    src/world/World.cpp
    src/engine/assets/TileAsset.cpp
    src/engine/assets/TileAssetLoader.cpp
    src/engine/assets/TileConnector.cpp
)

add_executable(RedClone ${REDCLONE_SOURCES})
target_include_directories(RedClone PRIVATE
    src
    ${mathfu_SOURCE_DIR}/include
    ${mathfu_SOURCE_DIR}/dependencies/vectorial/include
)
target_link_libraries(RedClone PRIVATE SFML::Graphics SFML::Window SFML::System Lua::Lua LuaCxx::LuaCxx)
target_compile_definitions(RedClone PRIVATE
    REDCLONE_ASSET_DIR="${CMAKE_SOURCE_DIR}/assets"
    $<$<CONFIG:Debug>:REDCLONE_DEBUG_LOGS=1>
)

add_subdirectory(tests)
