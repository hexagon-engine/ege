#include <ege/core.h>
#include <ege/util/system.h>

int main()
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
