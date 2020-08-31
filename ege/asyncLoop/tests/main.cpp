#include <testsuite/Tests.h>

#include <ege/asyncLoop/AsyncLoop.h>
#include <ege/asyncLoop/AsyncTask.h>
#include <ege/asyncLoop/ThreadSafeEventLoop.h>
#include <ege/loop/Timer.h>
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
    auto myTask = std::make_shared<EGE::AsyncTask>(myWorker, myCallback);
    loop.addAsyncTask(myTask, "myTask");

    bool running = true;

    while(running)
    {
        loop.onUpdate();
        sf::sleep(sf::seconds(0.25f));
        std::cerr << "main thread" << std::endl;

        if(myTask->finished())
        {
            auto timer = std::make_shared<EGE::Timer>(&loop, EGE::Timer::Mode::Limited, EGE::Time(1.0, EGE::Time::Unit::Seconds));
            timer->setCallback([&running](std::string name, EGE::Timer* timer) {
                                    running = false;
                               });
            loop.addTimer("exit", timer);
        }
    }
}

TESTCASE(threadSafeEventLoop)
{
    EGE::ThreadSafeEventLoop loop;
    float progress = 0;
    srand(time(NULL));
    bool running = true;

    auto loadingWorker = [&progress, &loop, &running]()->int {
        std::cerr << "Loading..." << std::endl;
        int mt = 14355;
        for(int i = 0; i < mt; i++)
        {
            progress = (float)i / mt;
            sf::sleep(sf::milliseconds(1));
            if(rand() % 100 == 0)
                std::cerr << "Doing something at i=" << i << std::endl;
            if(i == 5478)
                loop.deferredInvoke([&running]() {running = false; }); // It will be called in main thread.
        }
        return 0;
    };
    loop.addAsyncTask(std::make_shared<EGE::AsyncTask>(loadingWorker, [&running](EGE::AsyncTask::State state) {
        if(state.finished)
        {
            running = false;
        }
    }));
    auto timer = std::make_shared<EGE::Timer>(&loop, EGE::Timer::Mode::Infinite, EGE::Time(1.0, EGE::Time::Unit::Seconds));
    timer->setCallback([&progress](std::string timer, EGE::Timer* _timer) {
        std::cerr << "Loading progress: " << (int)(progress * 100) << "%" << std::endl;
    });
    loop.addTimer("displayProgress", timer);

    while(running)
        loop.onUpdate();
}

RUN_TESTS(asyncLoop);
