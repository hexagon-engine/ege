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

TESTCASE(resourceManager)
{
    EGE::GUIGameLoop gameLoop;
    auto window = gameLoop.openWindow(sf::VideoMode(300, 300), "EGE GUI Test (resourceManager)");
    gameLoop.setResourceManager(make<MyResourceManager>());
    window->setNewGUIScreen<MyGuiScreen>();
    return gameLoop.run();
}

RUN_TESTS(gui);
