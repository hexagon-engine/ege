#include <testsuite/Tests.h>
#include <ege/core.h>
#include <ege/debug.h>
#include <ege/event.h>

// reimplement that because we don't have GUIGameLoop here yet
class MyWndLoop : public EGE::MainLoop
{
public:
    virtual EGE::EventResult onLoad() override
    {
        m_window.create(sf::VideoMode(500, 500), "Keybind Test");
        return EGE::EventResult::Success;
    }

    virtual void onTick(EGE::TickCount) override
    {
        m_window.callEvents(*this);
    }

private:
    EGE::SFMLSystemWindow m_window;
};

TESTCASE(keybind)
{
    ege_log.info() << "keybind test started";
    ege_log.filterLevel(LogLevel::Debug, false);

    MyWndLoop loop;

    auto keybinds = make<EGE::KeybindManager>();
    keybinds->addTrigger("MyKeybind1", sf::Keyboard::A, []() { ege_log.info() << "MyKeybind1 pressed!"; });
    keybinds->addTrigger("MyKeybind2", sf::Mouse::Left, []() { ege_log.info() << "MyKeybind2 pressed!"; });
    keybinds->addTrigger("MyKeybind3", sf::Mouse::VerticalWheel, []() { ege_log.info() << "MyKeybind3 pressed!"; });
    keybinds->addSwitch("MyKeybind3", sf::Mouse::VerticalWheel, [](bool pressed) { ege_log.info() << "MyKeybind3 pressed, pressed=" << pressed; });
    keybinds->addStrength("MyKeybind3", sf::Mouse::VerticalWheel, [](float value) { ege_log.info() << "MyKeybind3 pressed, value=" << value; });
    EGE::KeybindManager::hook(keybinds, loop);

    return loop.run();
}

RUN_TESTS(keybind)
