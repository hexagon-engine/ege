#include <testsuite/Tests.h>

#include <ege/tilemap/ChunkedTileMap2D.h>
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

TESTCASE(chunked)
{
    EGE::ChunkedTileMap2D<MyTile, 16, 16> tileMap;
    EXPECT_EQUAL(tileMap.getLocalCoords(EGE::Vec2i(16, 16)), EGE::Vec2s(0, 0));
    EXPECT_EQUAL(tileMap.getLocalCoords(EGE::Vec2i(-1, -1)), EGE::Vec2s(15, 15));
    EXPECT_EQUAL(tileMap.ensureTile(EGE::Vec2i(100, 100)).c, 123);
    EXPECT_EQUAL(tileMap.ensureTile(EGE::Vec2i(-165, -112)).c, 123);
    tileMap.ensureTile(EGE::Vec2i(-165, -112)).c = 100;
    DUMP(1, (int)tileMap.ensureTile(EGE::Vec2i(-165, -112)).c);
    EXPECT_EQUAL(tileMap.ensureTile(EGE::Vec2i(-165, -112)).c, 100);
    EXPECT_EQUAL(tileMap.size(), 2);
    return 0;
}

RUN_TESTS(tilemap)
