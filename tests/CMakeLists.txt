add_executable(RedCloneTests
    TestMain.cpp
    Camera2DTests.cpp
    TileMapTests.cpp
    SelectionControllerTests.cpp
    CameraControllerTests.cpp
    EcsTests.cpp
    WorldTests.cpp
    IsometricTests.cpp
    ScriptEngineTests.cpp
    LoggerTests.cpp
    TileAssetRegistryTests.cpp
    TileAssetMetadataTests.cpp
    TileConnectorTests.cpp
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
    ${CMAKE_SOURCE_DIR}/src/core/Logger.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/math/Isometric.cpp
    ${CMAKE_SOURCE_DIR}/src/ecs/EntityManager.cpp
    ${CMAKE_SOURCE_DIR}/src/world/World.cpp
    ${CMAKE_SOURCE_DIR}/src/scripting/ScriptEngine.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/assets/TileAsset.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/assets/TileAssetLoader.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/assets/TileConnector.cpp
)

target_link_libraries(RedCloneTests PRIVATE Lua::Lua LuaCxx::LuaCxx SFML::Graphics SFML::Window SFML::System)
target_compile_definitions(RedCloneTests PRIVATE
    REDCLONE_ASSET_DIR="${CMAKE_SOURCE_DIR}/assets"
    $<$<CONFIG:Debug>:REDCLONE_DEBUG_LOGS=1>
)

add_test(NAME RedCloneTests COMMAND RedCloneTests)
