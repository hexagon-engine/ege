#include "ege/util/GlobalConfig.h"
#include <testsuite/Tests.h>
#include <ege/debug/Logger.h>
#include <ege/gui.h>
#include <cmath>

TESTCASE(simple)
{
    EGE::GUIGameLoop gameLoop;
    auto window = gameLoop.openWindow(sf::VideoMode(300, 300), "EGE GUI Test");
    window->setNewGUIScreen<EGE::GUIScreen>();
    window->setBackgroundColor(EGE::ColorRGBA::fromBytes(127, 127, 127));
    window->setFramerateLimit(60);
    return gameLoop.run();
}

TESTCASE(widget)
{
    EGE::GUIGameLoop gameLoop;
    auto window = gameLoop.openWindow(sf::VideoMode(300, 300), "EGE GUI Test");
    auto gui = window->setNewGUIScreen<EGE::GUIScreen>();

    auto widget = gui->addNewWidget<EGE::DummyWidget>();
    widget->setPosition(EGE::Vec2d(50.f, 50.f));
    widget->setSize(EGE::Vec2d(50.f, 50.f));

    auto widget2 = gui->addNewWidget<EGE::DummyWidget>();
    widget2->setPosition(EGE::Vec2d(150.f, 50.f));
    widget2->setSize(EGE::Vec2d(50.f, 50.f));

    return gameLoop.run();
}

TESTCASE(guiChange)
{
    EGE::GUIGameLoop gameLoop;
    auto window = gameLoop.openWindow(sf::VideoMode(300, 300), "EGE GUI Test");

    auto gui = window->setNewGUIScreen<EGE::GUIScreen>();

    auto widget = gui->addNewWidget<EGE::DummyWidget>();
    widget->setPosition(EGE::Vec2d(50.f, 50.f));
    widget->setSize(EGE::Vec2d(50.f, 50.f));

    auto widget2 = gui->addNewWidget<EGE::DummyWidget>();
    widget2->setPosition(EGE::Vec2d(150.f, 50.f));
    widget2->setSize(EGE::Vec2d(50.f, 50.f));

    auto gui2 = make<EGE::GUIScreen>(*window);

    auto widget3 = gui->addNewWidget<EGE::DummyWidget>();
    widget3->setPosition(EGE::Vec2d(50.f, 150.f));
    widget3->setSize(EGE::Vec2d(50.f, 50.f));

    auto widget4 = gui->addNewWidget<EGE::DummyWidget>();
    widget4->setPosition(EGE::Vec2d(150.f, 150.f));
    widget4->setSize(EGE::Vec2d(50.f, 50.f));

    window->addTimer("changeGUI", make<EGE::Timer>(gameLoop, EGE::Timer::Mode::Limited, EGE::Time(5.0, EGE::Time::Unit::Seconds), [gui2, window](std::string, EGE::Timer*) {
        window->setGUIScreen(gui2);
    }));

    return gameLoop.run();
}

class MyGuiScreen : public EGE::GUIScreen
{
public:
    EGE::SharedPtr<EGE::DummyWidget> widget1;
    sf::Font* font;
    EGE::Texture* texture;

    explicit MyGuiScreen(EGE::Window& window)
    : EGE::GUIScreen(window) {}

    virtual void onCreate() override
    {
        EGE::GUIScreen::onCreate();
        ege_log.info() << "MyResourceManager onCreate";
        widget1 = addNewWidget<EGE::DummyWidget>();
        widget1->setPosition(EGE::Vec2d(50.f, 50.f));
        widget1->setSize(EGE::Vec2d(50.f, 50.f));
        font = getResourceManager()->getFont("font.ttf").get();
        texture = getResourceManager()->getTexture("texture.png").get();
    }

    virtual void render(EGE::Renderer& renderer) const override
    {
        EGE::GUIScreen::render(renderer);
        ege_log.info() << "MyResourceManager render";
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
        ege_log.info() << "MyResourceManager reload";
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
    EGE::SharedPtr<EGE::Label> labelAnimated;
    EGE::SharedPtr<EGE::Label> labelFPS;
    EGE::SharedPtr<EGE::Button> button2;

    EGE::SharedPtr<AnimationGraphWidget> graph;
    EGE::SharedPtr<AnimationGraphWidget> graph2;
    EGE::SharedPtr<EGE::Label> ball;
    bool timerRunning = false;

    explicit MyGuiScreen2(EGE::Window& window)
    : EGE::GUIScreen(window) {}

    virtual void onCreate() override
    {
        EGE::GUIScreen::onCreate();

        ege_log.info() << "MyResourceManager onLoad";

        setPadding({10, 10});
        layoutDirection = EGE::LayoutElement::Direction::Vertical;

        labelFPS = addNewWidget<EGE::Label>();
        labelFPS->setString("FPS: 0.0");
        labelFPS->setTextAlign(EGE::Label::Align::Center);
        labelFPS->setSize({"1N", "20px"});

        auto widgets = addNewWidget<EGE::Widget>();

        {
            auto myFrame = widgets->addNewWidget<EGE::GroupBox>();
            myFrame->setLabel("Widget test");
            myFrame->layoutDirection = EGE::LayoutElement::Direction::Vertical;
            myFrame->setSize({"66%", "0N"});

            {
                auto button = myFrame->addNewWidget<EGE::Button>();
                button->setLabel("T.e.s.t&");
                button->events<EGE::Button::ClickEvent>().add([this, myFrame](EGE::Button::ClickEvent&) {
                    if(!timerRunning)
                    {
                        ege_log.info() << "clicked";

                        button2->hide(false);
                        timerRunning = true;
                        addTimer("TimerHideWidget", make<EGE::Timer>(*this, EGE::Timer::Mode::Limited, EGE::Time(1.0, EGE::Time::Unit::Seconds), [this, myFrame](std::string, EGE::Timer*) {
                            myFrame->removeWidget(*button2);
                            timerRunning = false;
                        }));
                    }
                    return EGE::EventResult::Success;
                });

                button2 = myFrame->addNewWidget<EGE::Button>();
                button2->setLabel("T.e.s.t&:2");
                button2->hide(true);

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

                auto radiogroup = myFrame->addNewWidget<EGE::RadioGroup>();
                radiogroup->setSize({"1N", "60px"});
                {
                    auto radioButton1 = radiogroup->addNewWidget<EGE::RadioButton>();
                    radioButton1->setLabel("RadioButton 1");
                    auto radioButton2 = radiogroup->addNewWidget<EGE::RadioButton>();
                    radioButton2->setLabel("RadioButton 2");
                    auto radioButton3 = radiogroup->addNewWidget<EGE::RadioButton>();
                    radioButton3->setLabel("RadioButton 3");
                    radiogroup->setCheckedButton(*radioButton2);
                    radiogroup->events<EGE::RadioGroupChangeEvent>().add([](EGE::RadioGroupChangeEvent& event) {
                        ege_log.info() << "RadioGroupChangeEvent: " << event.button.getLabel().toAnsiString();
                        return EGE::EventResult::Success;
                    });
                    ege_log.info() << radiogroup->getCheckedButton()->getLabel().toAnsiString();
                }

                auto slider = myFrame->addNewWidget<EGE::Slider>();
                slider->setMinValue(0.5);
                slider->events<EGE::SliderSlideEvent>().add([](EGE::SliderSlideEvent& event) {
                    ege_log.info() << "SliderSlideEvent: " << event.value;
                    return EGE::EventResult::Success;
                });
                auto spinBox = myFrame->addNewWidget<EGE::SpinBox>();
            }

            auto myFrame2 = widgets->addNewWidget<EGE::GroupBox>();
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
                listbox->addEntry("LB test 1");
                listbox->addEntry("LB test 2");
                listbox->addEntry("LB test 3");
                listbox->addEntry("LB test 4");
                listbox->addEntry("LB test 5");
                listbox->addEntry("LB test 6");
                listbox->addEntry("LB test 7");
                listbox->addEntry("LB test 8");
                listbox->addEntry("LB test 9");
                listbox->addEntry("LB test a");
                listbox->addEntry("LB test b");
                listbox->addEntry("LB test c");
                listbox->addEntry("LB test d");
                listbox->addEntry("LB test e");
                listbox->addEntry("LB test f");

                auto combobox = myFrame2->addNewWidget<EGE::ComboBox>();
                combobox->entries().addEntry("cb test 1");
                combobox->entries().addEntry("cb test 2");
                combobox->entries().addEntry("cb test 3");
                combobox->entries().addEntry("cb test 4");
                combobox->entries().addEntry("cb test 5");
                combobox->entries().addEntry("cb test 6");
                combobox->events<EGE::ListBox::SelectEvent>().add([combobox](EGE::ListBox::SelectEvent& ev) {
                    ege_log.info() << "selected: " << ev.value.toAnsiString();
                    return EGE::EventResult::Success;
                });

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
                addAnimation<EGE::MaxFloat>(anim3, [](EGE::NumberAnimation&, EGE::MaxFloat) {
                    //ball->setPosition(EGE::Vec2d(300.f, 400.f + val * 40.0));
                });

                auto animLabel = make<EGE::NumberAnimation>(*this, EGE::Time(1.0, EGE::Time::Unit::Seconds), EGE::Timer::Mode::Infinite);
                animLabel->addKeyframe(0.0, -1.0);
                animLabel->addKeyframe(0.5, 1.0);
                animLabel->addKeyframe(1.0, -1.0);
                animLabel->setEasingFunction(EGE::AnimationEasingFunctions::easeOutBounce);
                animLabel->setDelay(EGE::Time(2.0, EGE::Time::Unit::Seconds));
                addAnimation<EGE::MaxFloat>(animLabel, [](EGE::NumberAnimation&, EGE::MaxFloat) {
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

    virtual void onTick() override
    {
        labelFPS->setString("FPS: " + std::to_string(getLoop().getLastTicksPerSecond()));
    }
};

class MyResourceManager2 : public EGE::ResourceManager
{
public:
    virtual bool reload() override
    {
        ege_log.info() << "MyResourceManager reload";
        setDefaultFont("font.ttf");
        return true;
    }
};

TESTCASE(resourceManager)
{
    EGE::GUIGameLoop gameLoop;
    auto window = gameLoop.openWindow(sf::VideoMode(300, 300), "EGE GUI Test (resourceManager)");
    gameLoop.setResourceManager(make<MyResourceManager>());
    window->setNewGUIScreen<MyGuiScreen>();
    return gameLoop.run();
}

TESTCASE(_widgets)
{
    EGE::GlobalConfig::enableAllDebug();
    //ege_log.filterLevel(LogLevel::Debug, false);
    EGE::GUIGameLoop gameLoop;
    auto window = gameLoop.openWindow(sf::VideoMode(500, 500), "EGE GUI Test (widgets)");
    gameLoop.setResourceManager(make<MyResourceManager2>());
    window->setNewGUIScreen<MyGuiScreen2>();
    window->setBackgroundColor(EGE::ColorRGBA::fromBytes(160, 160, 160));
    return gameLoop.run();
}

RUN_TESTS(gui);
