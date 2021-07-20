#include <ege/core.h>

using EGE::Timer;

int main()
{
    EGE::MainLoop loop;
    auto callback = [](std::string name, EGE::Timer*) {
                        std::cerr << name << std::endl;
                    };

    loop.addTimer("display-sec-count", make<Timer>(loop, Timer::Mode::Infinite, 1.0, [](std::string, Timer* timer) {
        std::cerr << timer->getIterationCount() << " seconds" << std::endl;
    }));

    loop.addTimer("single-shot 5s", make<Timer>(loop, Timer::Mode::Limited, 5.0, callback));

    auto timer = make<Timer>(loop, Timer::Mode::Limited, 5.0, callback);
    timer->setRemainingIterationCount(5);
    loop.addTimer("5-shots-ticks 5s", timer);

    loop.addTimer("infinite 2s", make<Timer>(loop, Timer::Mode::Infinite, 0.5, callback));

    loop.addTimer("stop-test", make<Timer>(loop, Timer::Mode::Limited, 10.0, [&loop](std::string, Timer*) {
        loop.getTimers("infinite 2s")[0].lock()->stop();
    }));
    loop.addTimer("restart-test", make<Timer>(loop, Timer::Mode::Limited, 12.0, [&loop](std::string, Timer*) {
        loop.getTimers("infinite 2s")[0].lock()->start();
    }));
    loop.addTimer("remove-test", make<Timer>(loop, Timer::Mode::Limited, 14.0, [&loop](std::string, Timer*) {
        loop.removeTimer("infinite 2s");
    }));
    loop.addTimer("close", make<Timer>(loop, Timer::Mode::Limited, 15.0, [&loop](std::string, Timer*) {
        loop.exit();
    }));
    loop.addTimer("test-events", make<Timer>(loop, Timer::Mode::Infinite, 1.0));

    loop.events<EGE::TimerStartEvent>().add([](EGE::TimerStartEvent& event) {
        std::cerr << "Timer started: " << event.timer.getName() << std::endl;
        return EGE::EventResult::Success;
    });
    loop.events<EGE::TimerTickEvent>().add([](EGE::TimerTickEvent& event) {
        std::cerr << "Timer tick: " << event.timer.getName() << std::endl;
        return EGE::EventResult::Success;
    });
    loop.events<EGE::TimerFinishEvent>().add([](EGE::TimerFinishEvent& event) {
        std::cerr << "Timer finished: " << event.timer.getName() << std::endl;
        return EGE::EventResult::Success;
    });

    return loop.run();
}
