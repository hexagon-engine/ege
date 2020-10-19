#include <testsuite/Tests.h>
#include <ege/profiler/Profiler.h>
#include <ege/profiler/ProfilerSectionStarter.h>
#include <iostream>

void sleep(float secs)
{
    timespec _tsReq;
    _tsReq.tv_nsec = (secs - (long long)secs) * 1000000000;
    _tsReq.tv_sec = (long long)secs;

    timespec _tsRem;
    EXPECT(nanosleep(&_tsReq, &_tsRem) <= 0);
}

TESTCASE(simple)
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

TESTCASE(restartTest)
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

RUN_TESTS(profiler)
