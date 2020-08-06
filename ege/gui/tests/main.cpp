#include <testsuite/Tests.h>
#include <ege/gui/GUIGameLoop.h>

class MyGameLoop : public EGE::GUIGameLoop
{
public:
    virtual EGE::EventResult onLoad() override
    {
        EGE::GUIGameLoop::onLoad();

        DEBUG_PRINT("onLoad");
        getWindow().lock().get()->setFramerateLimit(10);
        return EGE::EventResult::Success;
    }

    virtual void logicTick(long long tickCount) override
    {
        EGE::GUIGameLoop::logicTick(tickCount);
    }
};

TESTCASE(simple)
{
    MyGameLoop gameLoop;
    gameLoop.setWindow(std::make_shared<EGE::SFMLSystemWindow>(sf::VideoMode(300, 300), "EGE GUI Test"));
    gameLoop.setCurrentGUIScreen(new EGE::GUIScreen(&gameLoop));
    gameLoop.run();
}

TESTCASE(widget)
{
    MyGameLoop gameLoop;
    gameLoop.setWindow(std::make_shared<EGE::SFMLSystemWindow>(sf::VideoMode(300, 300), "EGE GUI Test"));
    auto gui = new EGE::GUIScreen(&gameLoop);

    auto widget = new EGE::DummyWidget(gui);
    widget->setPosition(sf::Vector2f(50.f, 50.f));
    widget->setSize(sf::Vector2f(50.f, 50.f));
    gui->addWidget(std::shared_ptr<EGE::Widget>(widget));

    auto widget2 = new EGE::DummyWidget(gui);
    widget2->setPosition(sf::Vector2f(150.f, 50.f));
    widget2->setSize(sf::Vector2f(50.f, 50.f));
    gui->addWidget(std::shared_ptr<EGE::Widget>(widget2));

    gameLoop.setCurrentGUIScreen(gui);
    gameLoop.run();
}

TESTCASE(guiChange)
{
    MyGameLoop gameLoop;
    gameLoop.setWindow(std::make_shared<EGE::SFMLSystemWindow>(sf::VideoMode(300, 300), "EGE GUI Test"));

    auto gui = new EGE::GUIScreen(&gameLoop);

    auto widget = new EGE::DummyWidget(gui);
    widget->setPosition(sf::Vector2f(50.f, 50.f));
    widget->setSize(sf::Vector2f(50.f, 50.f));
    gui->addWidget(std::shared_ptr<EGE::Widget>(widget));

    auto widget2 = new EGE::DummyWidget(gui);
    widget2->setPosition(sf::Vector2f(150.f, 50.f));
    widget2->setSize(sf::Vector2f(50.f, 50.f));
    gui->addWidget(std::shared_ptr<EGE::Widget>(widget2));

    auto gui2 = new EGE::GUIScreen(&gameLoop);

    auto widget3 = new EGE::DummyWidget(gui);
    widget3->setPosition(sf::Vector2f(50.f, 150.f));
    widget3->setSize(sf::Vector2f(50.f, 50.f));
    gui2->addWidget(std::shared_ptr<EGE::Widget>(widget3));

    auto widget4 = new EGE::DummyWidget(gui);
    widget4->setPosition(sf::Vector2f(150.f, 150.f));
    widget4->setSize(sf::Vector2f(50.f, 50.f));
    gui2->addWidget(std::shared_ptr<EGE::Widget>(widget4));

    gameLoop.setCurrentGUIScreen(gui);
    gameLoop.addTimer("changeGUI", &((new EGE::Timer(&gameLoop, EGE::Timer::Mode::Limited, EGE::Time(5.0, EGE::Time::Unit::Seconds)))
                                     ->setCallback([gui2, &gameLoop](std::string, EGE::Timer*) {
                                                    gameLoop.setCurrentGUIScreen(gui2);
                                                })), EGE::GameLoop::TimerImmediateStart::Yes);
    gameLoop.run();
}

RUN_TESTS(gui);
