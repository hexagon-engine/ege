#include <testsuite/Tests.h>
#include <ege/gui/GUIGameLoop.h>
#include <ege/gui/Button.h>
#include <ege/gui/Label.h>

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
    gameLoop.setCurrentGUIScreen(std::make_shared<EGE::GUIScreen>(&gameLoop));
    gameLoop.setBackgroundColor(sf::Color(127, 127, 127));
    gameLoop.run();
}

TESTCASE(widget)
{
    MyGameLoop gameLoop;
    gameLoop.setWindow(std::make_shared<EGE::SFMLSystemWindow>(sf::VideoMode(300, 300), "EGE GUI Test"));
    auto gui = std::make_shared<EGE::GUIScreen>(&gameLoop);

    auto widget = new EGE::DummyWidget(gui.get());
    widget->setPosition(sf::Vector2f(50.f, 50.f));
    widget->setSize(sf::Vector2f(50.f, 50.f));
    gui->addWidget(std::shared_ptr<EGE::Widget>(widget));

    auto widget2 = new EGE::DummyWidget(gui.get());
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

    auto gui = std::make_shared<EGE::GUIScreen>(&gameLoop);

    auto widget = new EGE::DummyWidget(gui.get());
    widget->setPosition(sf::Vector2f(50.f, 50.f));
    widget->setSize(sf::Vector2f(50.f, 50.f));
    gui->addWidget(std::shared_ptr<EGE::Widget>(widget));

    auto widget2 = new EGE::DummyWidget(gui.get());
    widget2->setPosition(sf::Vector2f(150.f, 50.f));
    widget2->setSize(sf::Vector2f(50.f, 50.f));
    gui->addWidget(std::shared_ptr<EGE::Widget>(widget2));

    auto gui2 = std::make_shared<EGE::GUIScreen>(&gameLoop);

    auto widget3 = new EGE::DummyWidget(gui.get());
    widget3->setPosition(sf::Vector2f(50.f, 150.f));
    widget3->setSize(sf::Vector2f(50.f, 50.f));
    gui2->addWidget(std::shared_ptr<EGE::Widget>(widget3));

    auto widget4 = new EGE::DummyWidget(gui.get());
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

    virtual void onLoad() override
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

    virtual void render(sf::RenderTarget& target) override
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
    virtual bool reload() override
    {
        DEBUG_PRINT("MyResourceManager reload");
        loadFontFromFile("font.ttf");
        loadTextureFromFile("texture.png");
        return true;
    }
};

class MyGuiScreen2 : public EGE::GUIScreen
{
public:
    std::shared_ptr<EGE::Button> button;
    std::shared_ptr<EGE::Button> button2;

    std::shared_ptr<EGE::Label> labelLeft;
    std::shared_ptr<EGE::Label> labelCenter;
    std::shared_ptr<EGE::Label> labelRight;
    bool timerRunning = false;

    MyGuiScreen2(MyGameLoop* loop)
    : EGE::GUIScreen(loop) {}

    virtual void onLoad() override
    {
        EGE::GUIScreen::onLoad();
        DEBUG_PRINT("MyResourceManager onLoad");

        button = std::make_shared<EGE::Button>(this);
        button->setLabel("T.e.s.t&");
        button->setPosition(sf::Vector2f(50.f, 50.f));
        button->setSize(sf::Vector2f(200.f, 40.f));
        addWidget(button);

        button2 = std::make_shared<EGE::Button>(this);
        button2->setLabel("T.e.s.t&:2");
        button2->setPosition(sf::Vector2f(50.f, 100.f));
        button2->setSize(sf::Vector2f(200.f, 40.f));

        labelLeft = std::make_shared<EGE::Label>(this);
        labelLeft->setString("Label Left");
        labelLeft->setTextPosition(sf::Vector2f(10.f, 150.f));
        addWidget(labelLeft);

        labelCenter = std::make_shared<EGE::Label>(this);
        labelCenter->setString("Label Center");
        labelCenter->setTextPosition(sf::Vector2f(150.f, 200.f));
        labelCenter->setTextAlign(EGE::Label::Align::Center);
        addWidget(labelCenter);

        labelRight = std::make_shared<EGE::Label>(this);
        labelRight->setString("Label Right");
        labelRight->setTextPosition(sf::Vector2f(290.f, 250.f));
        labelRight->setTextAlign(EGE::Label::Align::Right);
        addWidget(labelRight);
    }

    virtual void onCommand(const EGE::Widget::Command& command) override
    {
        if(command.getId() == "EGE::Button::Command")
        {
            EGE::Button::Command* bCommand = (EGE::Button::Command*)&command;
            if(bCommand->getButton() == button.get() && !timerRunning)
            {
                DEBUG_PRINT("clicked");
                addWidget(button2);
                timerRunning = true;
                getLoop()->addTimer("TimerHideWidget", &(new EGE::Timer(getLoop(), EGE::Timer::Mode::Limited, EGE::Time(1.0, EGE::Time::Unit::Seconds)))->setCallback(
                    [this](std::string name, EGE::Timer* timer) {
                        removeWidget(button2.get());
                        timerRunning = false;
                    }
                ));
            }
        }
    }

    virtual void render(sf::RenderTarget& target)
    {
        EGE::GUIScreen::render(target);

    }
};

class MyResourceManager2 : public EGE::ResourceManager
{
public:
    virtual bool reload() override
    {
        DEBUG_PRINT("MyResourceManager reload");
        setDefaultFont("font.ttf");
        return true;
    }
};

TESTCASE(resourceManager)
{
    MyGameLoop gameLoop;
    gameLoop.setWindow(std::make_shared<EGE::SFMLSystemWindow>(sf::VideoMode(300, 300), "EGE GUI Test (resourceManager)"));
    gameLoop.setResourceManager(std::make_shared<MyResourceManager>());
    gameLoop.setCurrentGUIScreen(std::make_shared<MyGuiScreen>(&gameLoop));
    gameLoop.run();
}

TESTCASE(widgets)
{
    MyGameLoop gameLoop;
    gameLoop.setWindow(std::make_shared<EGE::SFMLSystemWindow>(sf::VideoMode(300, 300), "EGE GUI Test (widgets)"));
    gameLoop.setResourceManager(std::make_shared<MyResourceManager2>());
    gameLoop.setCurrentGUIScreen(std::make_shared<MyGuiScreen2>(&gameLoop));
    gameLoop.run();
}

RUN_TESTS(gui);
