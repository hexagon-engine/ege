#include <testsuite/Tests.h>
#include <ege/util/Object.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/ObjectString.h>

TESTCASE(object)
{
    EGE::ObjectMap map;
    map.addObject("test", std::make_shared<EGE::ObjectString>("test"));
    map.addObject("test2", std::make_shared<EGE::ObjectString>("test"));
    auto map2 = std::make_shared<EGE::ObjectMap>();
    map2->addObject("test3", std::make_shared<EGE::ObjectString>("test44"));
    map.addObject("testObjects", map2);
    DEBUG_PRINT(map.getObject("test").lock()->toString().c_str());
    DEBUG_PRINT(((EGE::ObjectMap*)map.getObject("testObjects").lock().get())->getObject("test3").lock()->toString().c_str());
    DEBUG_PRINT(map.toString().c_str());
}

RUN_TESTS(util)
