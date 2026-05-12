add_executable(RedCloneTests
    Camera2DTests.cpp
)

target_include_directories(RedCloneTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${mathfu_SOURCE_DIR}/include
    ${mathfu_SOURCE_DIR}/dependencies/vectorial/include
)

target_sources(RedCloneTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/engine/camera/Camera2D.cpp
    ${CMAKE_SOURCE_DIR}/src/world/TileMap.cpp
    ${CMAKE_SOURCE_DIR}/src/world/TileMapGenerator.cpp
    ${CMAKE_SOURCE_DIR}/src/gameplay/SelectionController.cpp
    ${CMAKE_SOURCE_DIR}/src/core/CameraController.cpp
)

add_test(NAME RedCloneTests COMMAND RedCloneTests)
