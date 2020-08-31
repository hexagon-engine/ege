#include <testsuite/Tests.h>

#include <ege/asyncLoop/AsyncLoop.h>
#include <ege/asyncLoop/AsyncTask.h>
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

RUN_TESTS(asyncLoop);
