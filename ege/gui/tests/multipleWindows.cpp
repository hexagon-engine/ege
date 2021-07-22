#include <testsuite/Tests.h>
#include <ege/gui.h>
#include <ege/util.h>

TESTCASE(basic)
{
    EGE::GlobalConfig::enableAllDebug();
    EGE::GUIGameLoop loop;

    auto window1 = loop.openWindow(sf::VideoMode(200, 200), "Window 1");
    window1->setBackgroundColor(EGE::Colors::yellow);
    auto guiScreen1 = window1->setNewGUIScreen<EGE::GUIScreen>();
    guiScreen1->setPadding({"4px", "4px"});
    auto button1 = guiScreen1->addNewWidget<EGE::Button>();
    button1->events<EGE::Button::ClickEvent>().add([window1](auto&) {
        ege_log.info() << "Button1 clicked!";
        window1->close();
        return EGE::EventResult::Success;
    });

    auto window2 = loop.openWindow(sf::VideoMode(200, 200), "Window 2");
    window2->setBackgroundColor(EGE::Colors::cyan);
    auto guiScreen2 = window2->setNewGUIScreen<EGE::GUIScreen>();
    guiScreen2->setPadding({"4px", "4px"});
    auto button2 = guiScreen2->addNewWidget<EGE::CheckBox>();
    button2->events<EGE::Button::ClickEvent>().add([window2](auto&) {
        ege_log.info() << "Button2 clicked!";
        window2->close();
        return EGE::EventResult::Success;
    });

    return loop.run();
}

RUN_TESTS(multipleWindows)
