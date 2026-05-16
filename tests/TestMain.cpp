int runCamera2DTests();
int runTileMapTests();
int runTerrainMeshTests();
int runSelectionControllerTests();
int runCameraControllerTests();
int runEcsTests();
int runWorldTests();
int runIsometricTests();
int runScriptEngineTests();
int runLoggerTests();
int runTileAssetTests();
int runTileAssetRegistryTests();
int runTileAssetMetadataTests();
int runTileConnectorTests();

int main()
{
    if (runCamera2DTests() != 0)
    {
        return 1;
    }
    if (runTileMapTests() != 0)
    {
        return 1;
    }
    if (runTerrainMeshTests() != 0)
    {
        return 1;
    }
    if (runSelectionControllerTests() != 0)
    {
        return 1;
    }
    if (runCameraControllerTests() != 0)
    {
        return 1;
    }
    if (runEcsTests() != 0)
    {
        return 1;
    }
    if (runWorldTests() != 0)
    {
        return 1;
    }
    if (runIsometricTests() != 0)
    {
        return 1;
    }
    if (runScriptEngineTests() != 0)
    {
        return 1;
    }
    if (runLoggerTests() != 0)
    {
        return 1;
    }

    if (runTileAssetTests() != 0)
    {
        return 1;
    }
    if (runTileAssetRegistryTests() != 0)
    {
        return 1;
    }
    if (runTileAssetMetadataTests() != 0)
    {
        return 1;
    }
    if (runTileConnectorTests() != 0)
    {
        return 1;
    }

    return 0;
}
