int runCamera2DTests();
int runTileMapTests();
int runSelectionControllerTests();
int runCameraControllerTests();

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
    if (runSelectionControllerTests() != 0)
    {
        return 1;
    }
    if (runCameraControllerTests() != 0)
    {
        return 1;
    }

    return 0;
}
