#include <testsuite/Tests.h>
#include <ege/gui/Animation.h>
#include <ege/gui/AnimationEasingFunctions.h>
#include <ege/gui/GUIGameLoop.h>
#include <ege/gui/Button.h>
#include <ege/gui/CheckBox.h>
#include <ege/gui/Frame.h>
#include <ege/gui/Label.h>
#include <ege/gui/RadioButton.h>
#include <ege/gui/TextBox.h>
#include <cmath>

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
    gameLoop.addTimer("changeGUI", std::shared_ptr<EGE::Timer>(&(new EGE::Timer(&gameLoop, EGE::Timer::Mode::Limited, EGE::Time(5.0, EGE::Time::Unit::Seconds)))
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

class AnimationGraphWidget : public EGE::Widget
{
    std::vector<double> m_vals;
    int m_max = 1;
public:
    AnimationGraphWidget(EGE::Widget* parent)
    : EGE::Widget(parent) {}

    void setMax(int m)
    {
        m_max = m;
    }

    void addVal(double val)
    {
        m_vals.push_back(val);
    }

    void setSize(sf::Vector2f size)
    {
        m_size = size;
    }

    void render(sf::RenderTarget& target)
    {
        Widget::render(target);

        sf::RectangleShape rs(m_size - sf::Vector2f(2.f, 2.f));
        rs.setPosition(1.f, 1.f);
        rs.setOutlineThickness(1.f);
        rs.setOutlineColor(sf::Color::White);
        rs.setFillColor(sf::Color(255, 255, 255, 30));
        target.draw(rs);

        sf::VertexArray varr(sf::LineStrip);
        int c = 0;
        for(double v: m_vals)
        {
            varr.append(sf::Vertex(sf::Vector2f((double)c * m_size.x / m_max, v * (m_size.y / 12.f) + m_size.y / 2.f), sf::Color::Red));
            c++;
        }
        target.draw(varr);
    }
};

class MyGuiScreen2 : public EGE::GUIScreen
{
public:
    std::shared_ptr<EGE::Button> button;
    std::shared_ptr<EGE::Button> button2;

    std::shared_ptr<EGE::Label> labelAnimated;
    std::shared_ptr<EGE::Label> labelFPS;

    std::shared_ptr<EGE::TextBox> myTextBox;
    std::shared_ptr<EGE::CheckBox> checkBox;
    std::shared_ptr<EGE::RadioButton> radioButton;

    std::shared_ptr<AnimationGraphWidget> graph;
    std::shared_ptr<AnimationGraphWidget> graph2;
    std::shared_ptr<EGE::Label> ball;
    bool timerRunning = false;

    MyGuiScreen2(MyGameLoop* loop)
    : EGE::GUIScreen(loop) {}

    virtual void onLoad() override
    {
        EGE::GUIScreen::onLoad();
        getWindow().lock()->setFramerateLimit(60);
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

        auto labelLeft = std::make_shared<EGE::Label>(this);
        labelLeft->setString("Label Left");
        labelLeft->setPosition(sf::Vector2f(10.f, 150.f));
        addWidget(labelLeft);

        auto labelCenter = std::make_shared<EGE::Label>(this);
        labelCenter->setString("Label Center");
        labelCenter->setPosition(sf::Vector2f(150.f, 200.f));
        labelCenter->setTextAlign(EGE::Label::Align::Center);
        addWidget(labelCenter);

        auto labelRight = std::make_shared<EGE::Label>(this);
        labelRight->setString("Label Right");
        labelRight->setPosition(sf::Vector2f(290.f, 250.f));
        labelRight->setTextAlign(EGE::Label::Align::Right);
        addWidget(labelRight);

        labelAnimated = std::make_shared<EGE::Label>(this);
        labelAnimated->setString("Animation");
        labelAnimated->setPosition(sf::Vector2f(150.f, 300.f));
        labelAnimated->setTextAlign(EGE::Label::Align::Center);
        addWidget(labelAnimated);

        ball = std::make_shared<EGE::Label>(this);
        ball->setString("O");
        ball->setPosition(sf::Vector2f(0.f, 0.f));
        ball->setTextAlign(EGE::Label::Align::Center);
        addWidget(ball);

        labelFPS = std::make_shared<EGE::Label>(this);
        labelFPS->setString("FPS: 0.0");
        labelFPS->setPosition(sf::Vector2f(10.f, 10.f));
        labelFPS->setTextAlign(EGE::Label::Align::Center);
        addWidget(labelFPS);

        myTextBox = std::make_shared<EGE::TextBox>(this);
        myTextBox->setPosition(sf::Vector2f(20.f, 400.f));
        myTextBox->setSize(sf::Vector2f(460.f, 25.f));
        addWidget(myTextBox);

        auto myFrame = std::make_shared<EGE::Frame>(this);
        myFrame->setSize(sf::Vector2f(480.f, 470.f));
        myFrame->setPosition(sf::Vector2f(10.f, 20.f));
        myFrame->setLabel("Widget test");
        addWidget(myFrame);

        checkBox = std::make_shared<EGE::CheckBox>(this);
        checkBox->setPosition(sf::Vector2f(20.f, 440.f));
        checkBox->setLabel("CheckBox");
        addWidget(checkBox);

        radioButton = std::make_shared<EGE::RadioButton>(this);
        radioButton->setPosition(sf::Vector2f(20.f, 460.f));
        radioButton->setLabel("RadioButton");
        addWidget(radioButton);

        graph = std::make_shared<AnimationGraphWidget>(this);
        graph->setPosition(sf::Vector2f(300.f, 100.f));
        graph->setSize(sf::Vector2f(100.f, 100.f));
        graph->setMax(600.f);
        addWidget(graph);

        auto anim = std::make_shared<EGE::Animation>(this, EGE::Time(10.0, EGE::Time::Unit::Seconds));
        anim->addKeyframe(0.0, 1.0);
        anim->addKeyframe(0.1, 5.0);
        anim->addKeyframe(0.5, -3.0);
        anim->addKeyframe(1.0, 6.0);
        addAnimation(anim, [this](EGE::Animation*, double val) {
                        graph->addVal(val);
                     });

        graph2 = std::make_shared<AnimationGraphWidget>(this);
        graph2->setPosition(sf::Vector2f(300.f, 210.f));
        graph2->setSize(sf::Vector2f(100.f, 100.f));
        graph2->setMax(600.f);
        addWidget(graph2);

        auto anim2 = std::make_shared<EGE::Animation>(this, EGE::Time(10.0, EGE::Time::Unit::Seconds));
        anim2->addKeyframe(0.0, 1.0);
        anim2->addKeyframe(0.1, 5.0);
        anim2->addKeyframe(0.5, -3.0);
        anim2->addKeyframe(1.0, 6.0);
        anim2->setEasingFunction(EGE::AnimationEasingFunctions::easeInOutCirc);
        addAnimation(anim2, [this](EGE::Animation*, double val) {
                        graph2->addVal(val);
                     });

        auto anim3 = std::make_shared<EGE::Animation>(this, EGE::Time(75, EGE::Time::Unit::Ticks), EGE::Timer::Mode::Infinite);
        anim3->addKeyframe(0.0, -1.0);
        anim3->addKeyframe(1.0, 1.0);
        anim3->setEasingFunction([](double x)->double {
                                                return ((x-0.5)*(x-0.5));
                                                } );
        addAnimation(anim3, [this](EGE::Animation*, double val) {
                        ball->setPosition(sf::Vector2f(300.f, 400.f + val * 40.0));
                     });

        auto animLabel = std::make_shared<EGE::Animation>(this, EGE::Time(1.0, EGE::Time::Unit::Seconds), EGE::Timer::Mode::Infinite);
        animLabel->addKeyframe(0.0, -1.0);
        animLabel->addKeyframe(0.5, 1.0);
        animLabel->addKeyframe(1.0, -1.0);
        animLabel->setEasingFunction(EGE::AnimationEasingFunctions::easeOutBounce);
        animLabel->setDelay(EGE::Time(2.0, EGE::Time::Unit::Seconds));
        addAnimation(animLabel, [this](EGE::Animation*, double val) {
                        labelAnimated->setPosition(sf::Vector2f(150.f + val * 30.f, 300.f));
                     });
    }

    virtual void onResize(sf::Event::SizeEvent& event)
    {
        labelFPS->setPosition(sf::Vector2f(event.width / 2.f, 10.f));
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
                addTimer("TimerHideWidget", &(new EGE::Timer(this, EGE::Timer::Mode::Limited, EGE::Time(1.0, EGE::Time::Unit::Seconds)))->setCallback(
                    [this](std::string, EGE::Timer*) {
                        removeWidget(button2.get());
                        timerRunning = false;
                    }
                ));
            }
        }
    }

    virtual void onUpdate(long long tickCounter)
    {
        EGE::GUIScreen::onUpdate(tickCounter);
        labelFPS->setString("FPS: " + std::to_string((int)(1.f / getLoop()->getLatestFrameTime().asSeconds())));
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

TESTCASE(_widgets)
{
    MyGameLoop gameLoop;
    gameLoop.setWindow(std::make_shared<EGE::SFMLSystemWindow>(sf::VideoMode(500, 500), "EGE GUI Test (widgets)"));
    gameLoop.setResourceManager(std::make_shared<MyResourceManager2>());
    gameLoop.setCurrentGUIScreen(std::make_shared<MyGuiScreen2>(&gameLoop));
    gameLoop.setBackgroundColor(sf::Color(209, 200, 192));
    gameLoop.run();
}

RUN_TESTS(gui);
