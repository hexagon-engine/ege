#include <testsuite/Tests.h>
#include <ege/geometry/Point.h>
#include <ege/geometry/Shape.h>
#include <ege/geometry/ShapeSet.h>
#include <ege/main/Config.h>

TESTCASE(simple)
{
    auto point = std::make_shared<EGE::Point>(10, 43);
    EXPECT_EQUAL(point->x, 10);
    EXPECT_EQUAL(point->y, 43);
    auto pointSum = (std::make_shared<EGE::Point>(43, 10))->sum(point);
    EXPECT(!pointSum->isEmpty());
    EXPECT_EQUAL(pointSum->size(), 2);
    DUMP(1, pointSum->size());
    auto pointIntersection = (std::make_shared<EGE::Point>(43, 10))->intersection(point);
    EXPECT(pointIntersection->isEmpty());
    DUMP(1, pointIntersection->size());
    auto pointIntersection2 = (std::make_shared<EGE::Point>(10, 43))->intersection(point);
    EXPECT(!pointIntersection2->isEmpty());
    DUMP(1, pointIntersection2->size());
    auto pointSum2 = pointSum->sum(pointIntersection2);
    EXPECT(!pointSum2->isEmpty());
    EXPECT_EQUAL(pointSum2->size(), 3);
    DUMP(1, pointSum2->size());
}

RUN_TESTS(geometry);
