#include <testsuite/Tests.h>

#include <ege/core/MainLoop.h>
#include <ege/core/TickEvent.h>
#include <ege/core/Timer.h>
#include <ege/util/system.h>
#include <ege/util/PointerUtils.h>

int myWorker(EGE::AsyncTask&)
{
    for(int i = 0; i < 2; i++)
    {
        // TODO: Add EGE::System::sleep()
        EGE::System::sleep(EGE::System::ExactTime::fromSeconds(i));
        std::cerr << "worker: " << i << std::endl;
    }
    return 1;
}

void myCallback(EGE::AsyncTask::State state)
{
    std::cerr << "callback: finished=" << state.finished << " returnCode=" << state.returnCode << std::endl;
}

TESTCASE(asyncTask)
{
    EGE::MainLoop loop;
    auto myTask = make<EGE::AsyncTask>(myWorker, myCallback);
    loop.addAsyncTask(myTask, "myTask");
    loop.setMinimalTickTime(EGE::Time(0.25));
    loop.events<EGE::TickEvent>().add([&loop, myTask](auto&)->EGE::EventResult {
        std::cerr << "main thread" << std::endl;

        if(myTask->finished())
        {
            auto timer = make<EGE::Timer>(loop, EGE::Timer::Mode::Limited, 1.0, [&loop](std::string, EGE::Timer*) {
                loop.exit();
            });
            loop.addTimer("exit", timer);
        }
        return EGE::EventResult::Success;
    });
    return loop.run();
}

TESTCASE(eventLoopIsThreadSafe)
{
    EGE::MainLoop loop;
    loop.setMaxTicksPerSecond(30);
    float progress = 0;
    srand(time(NULL));

    auto loadingWorker = [&progress, &loop](EGE::AsyncTask& task)->int {
        std::cerr << "Loading..." << std::endl;
        int mt = 14355;
        for(int i = 0; i < mt; i++)
        {
            progress = (float)i / mt;
            EGE::System::sleep(EGE::System::ExactTime::fromMilliseconds(1));
            if(rand() % 100 == 0)
                std::cerr << "Doing something at i=" << i << std::endl;
            if(i == 5478)
                loop.exit();

            if(task.stopRequested())
                return 1;
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

RUN_TESTS(async);
