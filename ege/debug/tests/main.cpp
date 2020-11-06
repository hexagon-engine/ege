#include <testsuite/Tests.h>
#include <ege/debug/Dump.h>
#include <ege/debug/Logger.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/PointerUtils.h>
#include <ege/util/JSONConverter.h>
#include <fstream>
#include <iostream>

void printObject(std::string fileName)
{
    std::ifstream stream;
    stream.open(fileName);
    std::shared_ptr<EGE::Object> object = make<EGE::ObjectMap>();
    stream >> EGE::objectIn(object, EGE::JSONConverter());
    EXPECT(!stream.fail());
    EGE::printObject(object);
}

TESTCASE(_printObject)
{
    printObject("res/1.json");
    printObject("res/2.json");
    return 0;
}

TESTCASE(logger)
{
    err(LogLevel::Debug) << "Test " << 1234;
    err(LogLevel::Verbose) << "Test " << 1234;
    err(LogLevel::Info) << "Test " << 1234;
    err(LogLevel::Warning) << "Test " << 1234;
    err(LogLevel::Error) << "Test " << 1234;
    err(LogLevel::Critical) << "Test " << 1234;
    err(LogLevel::Crash) << "Test " << 1234;

    return 0;
}

RUN_TESTS(debug)
