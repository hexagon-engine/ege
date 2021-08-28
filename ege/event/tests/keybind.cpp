#include <testsuite/Tests.h>
#include <ege/core.h>
#include <ege/debug.h>
#include <ege/event.h>

// NOTE: This test won't render anything and will appear to be
// hanged, but it's okay.

// reimplement that because we don't have GUIGameLoop here yet
class MyWndLoop : public EGE::MainLoop
{
public:
    virtual EGE::EventResult onLoad() override
    {
        m_window.create(sf::VideoMode(500, 500), "Keybind Test");
        return EGE::EventResult::Success;
    }

    virtual void onTick() override
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
    keybinds->setKeybind("MyKeybind1", sf::Keyboard::A);
    keybinds->setKeybind("MyKeybind2", sf::Mouse::Left);
    keybinds->setKeybind("MyKeybind3", sf::Mouse::VerticalWheel);

    auto& handler = EGE::KeybindManager::hook(keybinds, loop);
    handler.addTriggerHandler("MyKeybind1", []() { ege_log.info() << "MyKeybind1 triggered!"; });
    handler.addTriggerHandler("MyKeybind2", []() { ege_log.info() << "MyKeybind2 triggered!"; });
    handler.addTriggerHandler("MyKeybind3", []() { ege_log.info() << "MyKeybind3 triggered!"; });
    handler.addSwitchHandler("MyKeybind3", [](bool pressed) { ege_log.info() << "MyKeybind3 switched, pressed=" << pressed; });
    handler.addStrengthHandler("MyKeybind3", [](float value) { ege_log.info() << "MyKeybind3 changed, value=" << value; });

    return loop.run();
}

RUN_TESTS(keybind)
