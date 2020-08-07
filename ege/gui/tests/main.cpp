#include <testsuite/Tests.h>
#include <ege/gui/GUIGameLoop.h>

class MyGameLoop : public EGE::GUIGameLoop
{
public:
    virtual EGE::EventResult onLoad() override
    {
        EGE::EventResult result = EGE::GUIGameLoop::onLoad();
        if(result == EGE::EventResult::Failure)
            return result;

        DEBUG_PRINT("onLoad");
        getWindow().lock().get()->setFramerateLimit(60);
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

class MyGuiScreen : public EGE::GUIScreen
{
public:
    std::shared_ptr<EGE::DummyWidget> widget1;
    sf::Font* font;
    sf::Texture* texture;

    MyGuiScreen(MyGameLoop* loop)
    : EGE::GUIScreen(loop) {}

    virtual void onLoad()
    {
        EGE::GUIScreen::onLoad();
        DEBUG_PRINT("MyResourceManager onLoad");
        widget1 = std::make_shared<EGE::DummyWidget>(this);
        widget1->setPosition(sf::Vector2f(50.f, 50.f));
        widget1->setSize(sf::Vector2f(50.f, 50.f));
        addWidget(widget1);
        font = &*m_gameLoop->getResourceManager().lock()->getFont("font.ttf");
        texture = &*m_gameLoop->getResourceManager().lock()->getTexture("texture.png");
    }

    virtual void render(sf::RenderTarget& target)
    {
        EGE::GUIScreen::render(target);
        DEBUG_PRINT("MyResourceManager render");

        setViewForWidget(target);
        sf::Text text("TEST", *font, 30);
        target.draw(text);
        sf::Sprite sprite(*texture);
        sprite.setPosition(0.f, 40.f);
        target.draw(sprite);
    }
};
class MyResourceManager : public EGE::ResourceManager
{
public:
    virtual bool reload()
    {
        DEBUG_PRINT("MyResourceManager reload");
        loadFontFromFile("font.ttf");
        loadTextureFromFile("texture.png");
        return true;
    }
};

TESTCASE(resourceManager)
{
    MyGameLoop gameLoop;
    gameLoop.setWindow(std::make_shared<EGE::SFMLSystemWindow>(sf::VideoMode(300, 300), "EGE GUI Test (resourceManager)"));
    gameLoop.setResourceManager(std::make_shared<MyResourceManager>());
    gameLoop.setCurrentGUIScreen(new MyGuiScreen(&gameLoop));
    gameLoop.run();
}

RUN_TESTS(gui);
