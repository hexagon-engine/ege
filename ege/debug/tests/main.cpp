#include <testsuite/Tests.h>
#include <ege/debug/Dump.h>
#include <ege/debug/Logger.h>
#include <ege/debug/Profiler.h>
#include <ege/debug/ProfilerSectionStarter.h>
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

void sleep(float secs)
{
    timespec _tsReq;
    _tsReq.tv_nsec = (secs - (long long)secs) * 1000000000;
    _tsReq.tv_sec = (long long)secs;

    timespec _tsRem;
    EXPECT(nanosleep(&_tsReq, &_tsRem) <= 0);
}

TESTCASE(profilerSimple)
{
    EGE::Profiler profiler;
    profiler.start();

        profiler.startSection("test");
        sleep(0.25f);
        profiler.endStartSection("test2");
        sleep(0.25f);

            profiler.startSection("subTest1");
            sleep(0.25f);
            profiler.endStartSection("subTest2");
            sleep(0.5f);

                profiler.startSection("subsubTest1");
                sleep(0.25f);
                profiler.endStartSection("subsubtest2");
                sleep(0.125f);
                profiler.endSection();

            profiler.endSection();

        profiler.endSection();

    profiler.end();

    DEBUG_PRINT(("\n" + profiler.toString()).c_str());
    DEBUG_PRINT(("\n" + profiler.serialize()->toString()).c_str());

    return 0;
}

TESTCASE(profilerRestartTest)
{
    EGE::Profiler profiler;
    profiler.start();

        profiler.startSection("test");
        sleep(0.25f);
        profiler.endStartSection("test2");
        sleep(0.25f);

            profiler.startSection("subTest1");
            sleep(0.25f);
            profiler.endStartSection("subTest2");
            sleep(0.5f);

                profiler.startSection("subsubTest1_");
                sleep(0.25f);
                profiler.endStartSection("subsubtest2_");
                sleep(0.125f);
                profiler.endSection();

            profiler.endSection();

        profiler.endSection();

    profiler.end();

    profiler.start();

        profiler.startSection("test");
        sleep(0.25f);
        profiler.endStartSection("test2");
        sleep(0.25f);

            profiler.startSection("subTest1");
            sleep(0.25f);
            profiler.endStartSection("subTest2");
            sleep(0.5f);

                profiler.startSection("subsubTest1");
                sleep(0.25f);
                profiler.endStartSection("subsubtest2");
                sleep(0.125f);
                profiler.endSection();

            profiler.endSection();

        profiler.endSection();

    profiler.end();

    DEBUG_PRINT(("\n" + profiler.toString()).c_str());
    DEBUG_PRINT(("\n" + profiler.serialize()->toString()).c_str());

    return 0;
}

TESTCASE(profilerSectionStarter)
{
    EGE::Profiler profiler;
    profiler.start();

    sleep(0.25f);
    {
        EGE::ProfilerSectionStarter starter(profiler, "test1");
        sleep(0.5f);
        {
            EGE::ProfilerSectionStarter starter2(profiler, "subTest1");
            {
                EGE::ProfilerSectionStarter starter2(profiler, "subSubTest1");
                sleep(1.f);
            }
            sleep(0.75f);
        }
        sleep(0.375f);
    }

    profiler.end();

    DEBUG_PRINT(("\n" + profiler.toString()).c_str());
    DEBUG_PRINT(("\n" + profiler.serialize()->toString()).c_str());

    return 0;
}

RUN_TESTS(debug)
