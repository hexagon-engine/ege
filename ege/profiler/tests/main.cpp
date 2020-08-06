#include <testsuite/Tests.h>
#include <ege/profiler/Profiler.h>
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
}

RUN_TESTS(profiler)
