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

RUN_TESTS(gui);
