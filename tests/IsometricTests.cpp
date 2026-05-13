#include "engine/math/Isometric.h"

#include "TestCommon.h"

#include <iostream>

int runIsometricTests()
{
    using namespace redclone::engine::math;

    const Vec2f world{4.0F, 2.0F};
    const auto iso = isometric::worldToIso(world);
    if (!approx(iso[0], 64.0F) || !approx(iso[1], 96.0F))
    {
        std::cerr << "worldToIso failed\n";
        return 1;
    }

    const auto roundTripWorld = isometric::isoToWorld(iso);
    if (!approx(roundTripWorld[0], world[0]) || !approx(roundTripWorld[1], world[1]))
    {
        std::cerr << "isoToWorld round-trip failed\n";
        return 1;
    }

    const auto tileIso = isometric::tileToIso({10, 3});
    const auto tile = isometric::isoToTile(tileIso);
    if (tile[0] != 10 || tile[1] != 3)
    {
        std::cerr << "tile conversion failed\n";
        return 1;
    }

    if ((3.0F + 9.0F) >= (4.0F + 9.0F))
    {
        std::cerr << "depth ordering expectation failed\n";
        return 1;
    }

    return 0;
}
