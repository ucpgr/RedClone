#include "gameplay/SelectionController.h"

#include <iostream>

int runSelectionControllerTests()
{
    redclone::gameplay::SelectionController selection;
    selection.selectTile({1, 2});
    if (!selection.getSelectedTile().has_value())
    {
        std::cerr << "SelectionController select failed\n";
        return 1;
    }

    selection.clearSelection();
    if (selection.getSelectedTile().has_value())
    {
        std::cerr << "SelectionController clear failed\n";
        return 1;
    }

    return 0;
}
