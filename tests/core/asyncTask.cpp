#include <ege/core.h>
#include <ege/util/system.h>

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

int main()
{
    EGE::MainLoop loop;
    auto myTask = make<EGE::AsyncTask>(myWorker, myCallback);
    loop.addAsyncTask(myTask, "myTask");

    bool running = true;

    while(running)
    {
        loop.onUpdate();
        EGE::System::sleep(EGE::System::ExactTime::fromSeconds(0.25));
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
