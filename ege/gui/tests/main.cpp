#include <testsuite/Tests.h>
#include <ege/debug/Logger.h>
#include <ege/gui.h>
#include <cmath>

class MyGameLoop : public EGE::GUIGameLoop
{
public:
    virtual EGE::EventResult load()
    {
        DEBUG_PRINT("onLoad");
        getWindow().setFramerateLimit(60);
        return EGE::EventResult::Success;
    }
};

TESTCASE(simple)
{
    MyGameLoop gameLoop;
    gameLoop.openWindow(sf::VideoMode(300, 300), "EGE GUI Test");
    gameLoop.setCurrentGUIScreen(make<EGE::GUIScreen>(gameLoop));
    gameLoop.setBackgroundColor(sf::Color(127, 127, 127));
    return gameLoop.run();
}

TESTCASE(widget)
{
    MyGameLoop gameLoop;
    gameLoop.openWindow(sf::VideoMode(300, 300), "EGE GUI Test");
    auto gui = make<EGE::GUIScreen>(gameLoop);

    auto widget = make<EGE::DummyWidget>(*gui);
    widget->setPosition(EGE::Vec2d(50.f, 50.f));
    widget->setSize(EGE::Vec2d(50.f, 50.f));
    gui->addWidget(widget);

    auto widget2 = make<EGE::DummyWidget>(*gui);
    widget2->setPosition(EGE::Vec2d(150.f, 50.f));
    widget2->setSize(EGE::Vec2d(50.f, 50.f));
    gui->addWidget(widget2);

    gameLoop.setCurrentGUIScreen(gui);
    return gameLoop.run();
}

TESTCASE(guiChange)
{
    MyGameLoop gameLoop;
    gameLoop.openWindow(sf::VideoMode(300, 300), "EGE GUI Test");

    auto gui = make<EGE::GUIScreen>(gameLoop);

    auto widget = new EGE::DummyWidget(*gui);
    widget->setPosition(EGE::Vec2d(50.f, 50.f));
    widget->setSize(EGE::Vec2d(50.f, 50.f));
    gui->addWidget(std::shared_ptr<EGE::Widget>(widget));

    auto widget2 = new EGE::DummyWidget(*gui);
    widget2->setPosition(EGE::Vec2d(150.f, 50.f));
    widget2->setSize(EGE::Vec2d(50.f, 50.f));
    gui->addWidget(std::shared_ptr<EGE::Widget>(widget2));

    auto gui2 = make<EGE::GUIScreen>(gameLoop);

    auto widget3 = new EGE::DummyWidget(*gui);
    widget3->setPosition(EGE::Vec2d(50.f, 150.f));
    widget3->setSize(EGE::Vec2d(50.f, 50.f));
    gui2->addWidget(std::shared_ptr<EGE::Widget>(widget3));

    auto widget4 = new EGE::DummyWidget(*gui);
    widget4->setPosition(EGE::Vec2d(150.f, 150.f));
    widget4->setSize(EGE::Vec2d(50.f, 50.f));
    gui2->addWidget(std::shared_ptr<EGE::Widget>(widget4));

    gameLoop.setCurrentGUIScreen(gui);
    gameLoop.addTimer("changeGUI", make<EGE::Timer>(gameLoop, EGE::Timer::Mode::Limited, EGE::Time(5.0, EGE::Time::Unit::Seconds), [gui2, &gameLoop](std::string, EGE::Timer*) {
        gameLoop.setCurrentGUIScreen(gui2);
    }));

    return gameLoop.run();
}

class MyGuiScreen : public EGE::GUIScreen
{
public:
    std::shared_ptr<EGE::DummyWidget> widget1;
    sf::Font* font;
    EGE::Texture* texture;

    explicit MyGuiScreen(MyGameLoop& loop)
    : EGE::GUIScreen(loop) {}

    virtual void onLoad() override
    {
        EGE::GUIScreen::onLoad();
        DEBUG_PRINT("MyResourceManager onLoad");
        widget1 = make<EGE::DummyWidget>(*this);
        widget1->setPosition(EGE::Vec2d(50.f, 50.f));
        widget1->setSize(EGE::Vec2d(50.f, 50.f));
        addWidget(widget1);
        font = m_gameLoop.getResourceManager()->getFont("font.ttf").get();
        texture = m_gameLoop.getResourceManager()->getTexture("texture.png").get();
    }

    virtual void render(EGE::Renderer& renderer) const override
    {
        EGE::GUIScreen::render(renderer);
        DEBUG_PRINT("MyResourceManager render");
        sf::Text text("TEST", *font, 30);
        renderer.getTarget().draw(text);
        sf::Sprite sprite(texture->getTexture());
        sprite.setPosition(0.f, 40.f);
        renderer.getTarget().draw(sprite);
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
    explicit AnimationGraphWidget(EGE::Widget& parent)
    : EGE::Widget(parent) {}

    void setMax(int m)
    {
        m_max = m;
    }

    void addVal(double val)
    {
        m_vals.push_back(val);
    }

    void render(EGE::Renderer& renderer) const override
    {
        sf::RenderTarget& target = renderer.getTarget();

        sf::RectangleShape rs(sf::Vector2f(getSize().x - 2.f, getSize().y - 2.f));
        rs.setPosition(1.f, 1.f);
        rs.setOutlineThickness(1.f);
        rs.setOutlineColor(sf::Color::White);
        rs.setFillColor(sf::Color(255, 255, 255, 30));
        target.draw(rs);

        sf::VertexArray varr(sf::LineStrip);
        int c = 0;
        for(double v: m_vals)
        {
            varr.append(sf::Vertex(sf::Vector2f((double)c * getSize().x / m_max, v * (getSize().y / 12.f) + getSize().y / 2.f), sf::Color::Red));
            c++;
        }
        target.draw(varr);
    }
};

class MyGuiScreen2 : public EGE::GUIScreen
{
public:
    std::shared_ptr<EGE::Label> labelAnimated;
    std::shared_ptr<EGE::Label> labelFPS;
    std::shared_ptr<EGE::Button> button2;

    std::shared_ptr<AnimationGraphWidget> graph;
    std::shared_ptr<AnimationGraphWidget> graph2;
    std::shared_ptr<EGE::Label> ball;
    bool timerRunning = false;

    explicit MyGuiScreen2(MyGameLoop& loop)
    : EGE::GUIScreen(loop) {}

    virtual void onLoad() override
    {
        EGE::GUIScreen::onLoad();

        getWindow().setFramerateLimit(60);
        DEBUG_PRINT("MyResourceManager onLoad");

        setPadding({10, 10});
        layoutDirection = EGE::LayoutElement::Direction::Vertical;

        labelFPS = addNewWidget<EGE::Label>();
        labelFPS->setString("FPS: 0.0");
        labelFPS->setTextAlign(EGE::Label::Align::Center);
        labelFPS->setSize({"1N", "1a"});

        auto widgets = addNewWidget<EGE::CompoundWidget>();

        {
            auto myFrame = widgets->addNewWidget<EGE::Frame>();
            myFrame->setLabel("Widget test");
            myFrame->layoutDirection = EGE::LayoutElement::Direction::Vertical;
            myFrame->setSize({"66%", "0N"});

            {
                auto button = myFrame->addNewWidget<EGE::Button>();
                button->setLabel("T.e.s.t&");
                //button->setSize(EGE::Vec2d(200.f, 40.f));
                button->setCallback([this, myFrame]() {
                    if(!timerRunning)
                    {
                        DEBUG_PRINT("clicked");

                        myFrame->addWidget(button2);
                        timerRunning = true;
                        addTimer("TimerHideWidget", make<EGE::Timer>(*this, EGE::Timer::Mode::Limited, EGE::Time(1.0, EGE::Time::Unit::Seconds), [this, myFrame](std::string, EGE::Timer*) {
                            myFrame->removeWidget(button2.get());
                            timerRunning = false;
                        }));
                    }
                });

                button2 = make<EGE::Button>(*myFrame);
                button2->setLabel("T.e.s.t&:2");
                //button2->setSize(EGE::Vec2d(200.f, 40.f));

                auto labelLeft = myFrame->addNewWidget<EGE::Label>();
                labelLeft->setString("Label Left");

                auto labelCenter = myFrame->addNewWidget<EGE::Label>();
                labelCenter->setString("Label Center");
                labelCenter->setTextAlign(EGE::Label::Align::Center);

                auto labelRight = myFrame->addNewWidget<EGE::Label>();
                labelRight->setString("Label Right");
                labelRight->setTextAlign(EGE::Label::Align::Right);

                labelAnimated = myFrame->addNewWidget<EGE::Label>();
                labelAnimated->setString("Animation");
                labelAnimated->setTextAlign(EGE::Label::Align::Center);

                ball = myFrame->addNewWidget<EGE::Label>();
                ball->setString("O");
                ball->setTextAlign(EGE::Label::Align::Center);

                auto myTextBox = myFrame->addNewWidget<EGE::TextBox>();
                //myTextBox->setSize(EGE::Vec2d(440.f, 25.f));

                auto checkBox = myFrame->addNewWidget<EGE::CheckBox>();
                checkBox->setLabel("CheckBox");

                auto radioButton = myFrame->addNewWidget<EGE::RadioButton>();
                radioButton->setLabel("RadioButton");

                auto scrollBar = myFrame->addNewWidget<EGE::ScrollBar>();
                scrollBar->setPosition({"0px", "0px"});
                scrollBar->setType(EGE::ScrollBar::Type::Vertical);
                scrollBar->setLength(EGE::LayoutSizeD("100%"));
                scrollBar->setMaxValue(1.6);
                scrollBar->setUpdateCallback([](double val) {
                                                std::cerr << "scrollbar.value=" << val << std::endl;
                                             });

                auto spinBox = myFrame->addNewWidget<EGE::SpinBox>();
            }

            auto myFrame2 = widgets->addNewWidget<EGE::Frame>();
            myFrame2->setLabel("Animation test");
            myFrame2->layoutDirection = EGE::LayoutElement::Direction::Vertical;
            myFrame2->setSize({"1N", "1N"});

            {
                graph = myFrame2->addNewWidget<AnimationGraphWidget>();
                //graph->setSize(EGE::Vec2d(100.f, 100.f));
                graph->setMax(600.f);

                auto anim = make<EGE::NumberAnimation>(*this, EGE::Time(10.0, EGE::Time::Unit::Seconds));
                anim->addKeyframe(0.0, 1.0);
                anim->addKeyframe(0.1, 5.0);
                anim->addKeyframe(0.5, -3.0);
                anim->addKeyframe(1.0, 6.0);
                addAnimation<EGE::MaxFloat>(anim, [this](EGE::NumberAnimation&, EGE::MaxFloat val) {
                    graph->addVal(val);
                });

                graph2 = myFrame2->addNewWidget<AnimationGraphWidget>();
                //graph2->setSize(EGE::Vec2d(100.f, 100.f));
                graph2->setMax(600.f);

                auto listbox = myFrame2->addNewWidget<EGE::ListBox>();
                listbox->addNewEntry<EGE::Label>("LB test 1");
                listbox->addNewEntry<EGE::Label>("LB test 2");
                listbox->addNewEntry<EGE::Label>("LB test 3");
                listbox->addNewEntry<EGE::Label>("LB test 4");
                listbox->addNewEntry<EGE::Label>("LB test 5");
                listbox->addNewEntry<EGE::Label>("LB test 6");
                listbox->addNewEntry<EGE::Label>("LB test 7");
                listbox->addNewEntry<EGE::Label>("LB test 8");
                listbox->addNewEntry<EGE::Label>("LB test 9");
                listbox->addNewEntry<EGE::Label>("LB test a");
                listbox->addNewEntry<EGE::Label>("LB test b");
                listbox->addNewEntry<EGE::Label>("LB test c");
                listbox->addNewEntry<EGE::Label>("LB test d");
                listbox->addNewEntry<EGE::Label>("LB test e");

                auto anim2 = make<EGE::NumberAnimation>(*this, EGE::Time(10.0, EGE::Time::Unit::Seconds));
                anim2->addKeyframe(0.0, 1.0);
                anim2->addKeyframe(0.1, 5.0);
                anim2->addKeyframe(0.5, -3.0);
                anim2->addKeyframe(1.0, 6.0);
                anim2->setEasingFunction(EGE::AnimationEasingFunctions::easeInOutCirc);
                addAnimation<EGE::MaxFloat>(anim2, [this](EGE::NumberAnimation&, EGE::MaxFloat val) {
                    graph2->addVal(val);
                });

                auto anim3 = make<EGE::NumberAnimation>(*this, EGE::Time(75, EGE::Time::Unit::Ticks), EGE::Timer::Mode::Infinite);
                anim3->addKeyframe(0.0, -1.0);
                anim3->addKeyframe(1.0, 1.0);
                anim3->setEasingFunction([](double x)->double {
                    return ((x-0.5)*(x-0.5));
                } );
                addAnimation<EGE::MaxFloat>(anim3, [this](EGE::NumberAnimation&, EGE::MaxFloat) {
                    //ball->setPosition(EGE::Vec2d(300.f, 400.f + val * 40.0));
                });

                auto animLabel = make<EGE::NumberAnimation>(*this, EGE::Time(1.0, EGE::Time::Unit::Seconds), EGE::Timer::Mode::Infinite);
                animLabel->addKeyframe(0.0, -1.0);
                animLabel->addKeyframe(0.5, 1.0);
                animLabel->addKeyframe(1.0, -1.0);
                animLabel->setEasingFunction(EGE::AnimationEasingFunctions::easeOutBounce);
                animLabel->setDelay(EGE::Time(2.0, EGE::Time::Unit::Seconds));
                addAnimation<EGE::MaxFloat>(animLabel, [this](EGE::NumberAnimation&, EGE::MaxFloat) {
                    //labelAnimated->setPosition(EGE::Vec2d(150.f + val * 30.f, 300.f));
                });
            }
        }
    }

    virtual void onResize(sf::Event::SizeEvent& event) override
    {
        EGE::GUIScreen::onResize(event);
        //labelFPS->setPosition(EGE::Vec2d(event.width / 2.f, 10.f));
    }

    virtual void onUpdate(long long tickCounter) override
    {
        EGE::GUIScreen::onUpdate(tickCounter);
        //labelFPS->setString("FPS: " + std::to_string((int)(1.f / getLoop().getLatestFrameTime().asSeconds())));
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
    gameLoop.openWindow(sf::VideoMode(300, 300), "EGE GUI Test (resourceManager)");
    gameLoop.setResourceManager(make<MyResourceManager>());
    gameLoop.setCurrentGUIScreen(make<MyGuiScreen>(gameLoop));
    return gameLoop.run();
}

TESTCASE(_widgets)
{
    MyGameLoop gameLoop;
    gameLoop.openWindow(sf::VideoMode(500, 500), "EGE GUI Test (widgets)");
    gameLoop.setResourceManager(make<MyResourceManager2>());
    gameLoop.setCurrentGUIScreen(make<MyGuiScreen2>(gameLoop));
    gameLoop.setBackgroundColor(sf::Color(160, 160, 160));
    return gameLoop.run();
}

RUN_TESTS(gui);
