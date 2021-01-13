#include <testsuite/Tests.h>

#include <ege/asyncLoop/AsyncLoop.h>
#include <ege/asyncLoop/AsyncTask.h>
#include <ege/asyncLoop/ThreadSafeEventLoop.h>
#include <ege/loop/Timer.h>
#include <ege/util/PointerUtils.h>
#include <SFML/System.hpp>

int myWorker()
{
    for(int i = 0; i < 4; i++)
    {
        sf::sleep(sf::seconds(i));
        std::cerr << "worker: " << i << std::endl;
    }
    return 1;
}

void myCallback(EGE::AsyncTask::State state)
{
    std::cerr << "callback: finished=" << state.finished << " returnCode=" << state.returnCode << std::endl;
}

TESTCASE(simple)
{
    EGE::AsyncLoop loop;
    auto myTask = make<EGE::AsyncTask>(myWorker, myCallback);
    loop.addAsyncTask(myTask, "myTask");

    bool running = true;

    while(running)
    {
        loop.onUpdate();
        sf::sleep(sf::seconds(0.25f));
        std::cerr << "main thread" << std::endl;

        if(myTask->finished())
        {
            auto timer = make<EGE::Timer>(loop, EGE::Timer::Mode::Limited, 1.0, [&running](std::string, EGE::Timer*) {
                running = false;
            });
            loop.addTimer("exit", timer);
        }
    }

    return 0;
}

TESTCASE(threadSafeEventLoop)
{
    EGE::ThreadSafeEventLoop loop;
    float progress = 0;
    srand(time(NULL));

    auto loadingWorker = [&progress, &loop]()->int {
        std::cerr << "Loading..." << std::endl;
        int mt = 14355;
        for(int i = 0; i < mt; i++)
        {
            progress = (float)i / mt;
            sf::sleep(sf::milliseconds(1));
            if(rand() % 100 == 0)
                std::cerr << "Doing something at i=" << i << std::endl;
            if(i == 5478)
                loop.deferredInvoke([&loop]() { loop.exit(); }); // It will be called in main thread.
        }
        return 0;
    };
    loop.addAsyncTask(make<EGE::AsyncTask>(loadingWorker, [&loop](EGE::AsyncTask::State state) {
        if(state.finished)
        {
            loop.exit();
        }
    }));
    auto timer = make<EGE::Timer>(loop, EGE::Timer::Mode::Infinite, 1.0, [&progress](std::string, EGE::Timer*) {
        std::cerr << "Loading progress: " << (int)(progress * 100) << "%" << std::endl;
    });
    loop.addTimer("displayProgress", timer);
    return loop.run();
}

RUN_TESTS(asyncLoop);
