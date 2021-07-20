#include <testsuite/Tests.h>
#include <ege/debug/Dump.h>
#include <ege/debug/Logger.h>
#include <ege/debug/Profiler.h>
#include <ege/debug/ProfilerSectionStarter.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/PointerUtils.h>
#include <ege/util/JSONConverter.h>
#include <ege/util/Types.h>
#include <ege/util/system.h>
#include <fstream>
#include <iostream>

void printObject(std::string fileName)
{
    std::ifstream stream;
    stream.open(fileName);
    EGE::SharedPtr<EGE::Object> object = make<EGE::ObjectMap>();
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
    ege_err.debug() << "Test " << 1234;
    ege_err.verbose() << "Test " << 1234;
    ege_err.info() << "Test " << 1234;
    ege_err.warning() << "Test " << 1234;
    ege_err.error() << "Test " << 1234;
    ege_err.critical() << "Test " << 1234;
    ege_err.crash() << "Test " << 1234;

    ege_log.debug() << "Test " << 1234;
    ege_log.verbose() << "Test " << 1234;
    ege_log.info() << "Test " << 1234;
    ege_log.warning() << "Test " << 1234;
    ege_log.error() << "Test " << 1234;
    ege_log.critical() << "Test " << 1234;
    ege_log.crash() << "Test " << 1234;

    return 0;
}

void sleep(float secs)
{
    EGE::System::sleep(EGE::System::ExactTime::fromSeconds(secs));
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

    ege_log.info() << profiler.toString();
    ege_log.info() << profiler.serialize()->toString();

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

    ege_log.info() << profiler.toString();
    ege_log.info() << profiler.serialize()->toString();

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

    ege_log.info() << profiler.toString();
    ege_log.info() << profiler.serialize()->toString();

    return 0;
}

RUN_TESTS(debug)
