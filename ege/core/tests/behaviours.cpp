#include <testsuite/Tests.h>

#include <ege/core.h>

class TestMainLoop : public EGE::MainLoop
{
public:
    void increment() { m_counter++; }
    size_t value() const { return m_counter; }

private:
    size_t m_counter = 0;
};

TESTCASE(basic)
{
    TestMainLoop loop;
    loop.addNewBehaviour<EGE::SimpleBehaviour>([&loop](auto&) {
        ege_log.info() << "behaviour called! yay";
        loop.increment();
    });
    loop.events<EGE::TickEvent>().add([&](auto&) {
        ege_log.info() << "TickEvent called! yay";
        if(loop.getTickCount() == 10)
            loop.exit();
        return EGE::EventResult::Success;
    });
    EXPECT(loop.run() == 0);
    EXPECT(loop.value() == 10);
    return 0;
}

RUN_TESTS(behaviours)
