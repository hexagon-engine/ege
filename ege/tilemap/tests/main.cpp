#include <testsuite/Tests.h>

#include <ege/tilemap/FixedTileMap2D.h>

struct MyTile
{
    char c = 123;
};

TESTCASE(simple)
{
    EGE::FixedTileMap2D<MyTile, 16, 16> tileMap;
    tileMap.initialize();
    EXPECT_EQUAL(tileMap.getSize(), EGE::Vec2s(16, 16));
    EXPECT_EQUAL(tileMap.getTile({16, 16}), nullptr);
    EXPECT_EQUAL(tileMap.getTile({8, 8})->c, 123);
    tileMap.ensureTile({1, 1}).c = 55;
    EXPECT_EQUAL(tileMap.getTile({1, 1})->c, 55);
    return 0;
}

RUN_TESTS(tilemap)
