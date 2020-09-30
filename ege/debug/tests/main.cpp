#include <testsuite/Tests.h>
#include <ege/debug/Dump.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/PointerUtils.h>
#include <ege/util/JSONConverter.h>
#include <fstream>
#include <iostream>

TESTCASE(printObject)
{
    // parse
    std::ifstream stream;
    stream.open("res/1.json");
    std::shared_ptr<EGE::ObjectMap> map = make<EGE::ObjectMap>();
    stream >> EGE::objectIn(*map.get(), EGE::JSONConverter());
    EXPECT(!stream.fail());
    EGE::printObject(map);
}

RUN_TESTS(debug)
