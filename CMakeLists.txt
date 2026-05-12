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
FetchContent_MakeAvailable(sfml)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
FetchContent_Declare(
    mathfu
    GIT_REPOSITORY https://github.com/google/mathfu.git
    GIT_TAG v1.1.0
    GIT_SUBMODULES_RECURSE TRUE
)

FetchContent_GetProperties(mathfu)
if(NOT mathfu_POPULATED)
    FetchContent_Populate(mathfu)
endif()

set(REDCLONE_SOURCES
    src/core/Application.cpp
    src/core/CameraController.cpp
    src/core/GameLoop.cpp
    src/core/Logger.cpp
    src/ecs/Component.cpp
    src/ecs/EntityManager.cpp
    src/ecs/System.cpp
    src/engine/AssetManager.cpp
    src/engine/AudioSystem.cpp
    src/engine/camera/Camera2D.cpp
    src/engine/input/InputSystem.cpp
    src/engine/input/SfmlInputAdapter.cpp
    src/engine/Renderer.cpp
    src/engine/rendering/SfmlRenderer.cpp
    src/engine/window/Window.cpp
    src/gameplay/Building.cpp
    src/gameplay/Order.cpp
    src/gameplay/SelectionController.cpp
    src/gameplay/Unit.cpp
    src/gameplay/Weapon.cpp
    src/main.cpp
    src/scripting/ScriptEngine.cpp
    src/ui/UISystem.cpp
    src/world/Entity.cpp
    src/world/TileMap.cpp
    src/world/TileMapGenerator.cpp
    src/world/TileMapRenderer.cpp
    src/world/World.cpp
)

add_executable(RedClone ${REDCLONE_SOURCES})
target_include_directories(RedClone PRIVATE
    src
    ${mathfu_SOURCE_DIR}/include
    ${mathfu_SOURCE_DIR}/dependencies/vectorial/include
)
target_link_libraries(RedClone PRIVATE SFML::Graphics SFML::Window SFML::System)

add_subdirectory(tests)
