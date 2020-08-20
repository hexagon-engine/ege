#include <testsuite/Tests.h>
#include <ege/scene/Scene2D.h>
#include <ege/scene/SceneObject2D.h>
#include <ege/gui/Animation.h>
#include <ege/gui/AnimationEasingFunctions.h>
#include <ege/gui/GUIGameLoop.h>
#include <ege/gui/Label.h>

// my object definition
class MyObject : public EGE::SceneObject2D
{
    std::shared_ptr<sf::Font> m_font;

public:
    MyObject(EGE::Scene* owner, std::string name, sf::Vector2f pos)
    : EGE::SceneObject2D(owner, name), m_initialPosition(pos)
    {
        // set position from constructor
        setPosition(pos);

        // make object animated (it can be done now by flyTo())
        auto anim = std::make_shared<EGE::Animation>(this, EGE::Time(1.0, EGE::Time::Unit::Seconds), EGE::Timer::Mode::Infinite);
        anim->addKeyframe(0.0, -1.0);
        anim->addKeyframe(0.5, 1.0);
        anim->addKeyframe(1.0, -1.0);
        anim->setEasingFunction(EGE::AnimationEasingFunctions::easeInOutQuad);
        addAnimation(anim, [this](EGE::Animation*, double val) {
                        setPosition(m_initialPosition + sf::Vector2f(val * 20.f, 0));
                     });
    }

    void onUpdate(long long tickCounter)
    {
        SceneObject2D::onUpdate(tickCounter); // updateTimers()

        // because we can't use labels in Scene yet :(
        // it should be in render phase (it's not necessary on server)
        // maybe void renderUpdate() ??
        if(!m_font)
        {
            m_font = m_owner->getLoop()->getResourceManager().lock()->getDefaultFont();
            ASSERT(m_font);
        }

    }

    void render(sf::RenderTarget& target) const
    {
        // render "low-level" SFML text centered to object position
        sf::Text text("MyObject: " + getName(), *m_font, 18);
        text.setPosition(getPosition().x, getPosition().y);
        text.setOrigin((int)text.getLocalBounds().width / 2, (int)text.getLocalBounds().height / 2);
        target.draw(text);
    }
    void setDead()
    {
        m_dead = true;
    }

private:
    sf::Vector2f m_initialPosition;
};

class MyBackground : public EGE::SceneObject2D
{
public:
    MyBackground(EGE::Scene* owner, std::string name)
    : EGE::SceneObject2D(owner, name) {}

    void render(sf::RenderTarget& target) const
    {
        sf::VertexArray varr(sf::Quads, 4);
        varr.append(sf::Vertex(sf::Vector2f(-100.f, -100.f), sf::Color::Red));
        varr.append(sf::Vertex(sf::Vector2f(100.f, -100.f), sf::Color::Green));
        varr.append(sf::Vertex(sf::Vector2f(100.f, 100.f), sf::Color::Blue));
        varr.append(sf::Vertex(sf::Vector2f(-100.f, 100.f), sf::Color::Yellow));
        target.draw(varr);
    }
};

// resource manager
class MyResourceManager : public EGE::ResourceManager
{
public:
    // called on game start!
    virtual bool reload() override
    {
        // define default font to be used by EGE Widgets (we are using them in MyObject::render())
        // it will be automatically loaded by engine.
        bool success = setDefaultFont("font.ttf");

        // return true if setDefaultFont succeeded or false if not (e.g. font doesn't exist)
        return success;
    }
};

TESTCASE(simple)
{
    // create loop and window
    EGE::GUIGameLoop gameLoop;
    gameLoop.setWindow(std::make_shared<EGE::SFMLSystemWindow>(sf::VideoMode(300, 300), "EGE Scene Test"));

    // limit window framerate to 10
    gameLoop.getWindow().lock()->setFramerateLimit(60);

    // create main GUI
    auto gui = std::make_shared<EGE::GUIScreen>(&gameLoop);

    // create scene
    auto scene = std::make_shared<EGE::Scene>(gui.get());

    // add some objects to scene
    scene->addObject(std::make_shared<MyObject>(scene.get(), "My Object", sf::Vector2f(200.f, 200.f)));
    auto removedObject = std::make_shared<MyObject>(scene.get(), "Test Object", sf::Vector2f(100.f, 100.f));
    scene->addObject(removedObject);

    // set Test Object to be removed after 5 seconds
    removedObject->addTimer("makeTestObjectDead", &((new EGE::Timer(removedObject.get(), EGE::Timer::Mode::Limited, EGE::Time(5.0, EGE::Time::Unit::Seconds)))
                                     ->setCallback([scene, removedObject](std::string, EGE::Timer* timer) {
                                                   removedObject->setDead();
                                                })), EGE::GameLoop::TimerImmediateStart::Yes);

    // assign scene to GUI
    gui->addWidget(scene);

    // assign an instance of MyResourceManager to game loop
    gameLoop.setResourceManager(std::make_shared<MyResourceManager>());

    // assign GUI to loop
    gameLoop.setCurrentGUIScreen(gui);

    // run loop
    gameLoop.run();
}

TESTCASE(_2dCamera)
{
    // create loop and window
    EGE::GUIGameLoop gameLoop;
    gameLoop.setWindow(std::make_shared<EGE::SFMLSystemWindow>(sf::VideoMode(300, 300), "EGE Scene Test"));

    // limit window framerate to 10
    gameLoop.getWindow().lock()->setFramerateLimit(60);

    // create main GUI
    auto gui = std::make_shared<EGE::GUIScreen>(&gameLoop);

    // create scene
    auto scene = std::make_shared<EGE::Scene2D>(gui.get());

    // add camera
    auto cam = std::make_shared<EGE::CameraObject2D>(scene.get());
    bool b1 = false;

    // make camera animated
    auto timer = std::make_shared<EGE::Timer>(cam.get(), EGE::Timer::Mode::Infinite, EGE::Time(2.0, EGE::Time::Unit::Seconds));
    timer->setCallback([cam, &b1](std::string name, EGE::Timer* timer) {
                        cam->flyTo(b1 ? sf::Vector2f(0.f, 100.f) : sf::Vector2f(0.f, -100.f), 1.0);
                        b1 = !b1;
                       });
    // the first fly
    cam->flyTo(sf::Vector2f(0.f, 100.f), 1.0);
    cam->addTimer("camera fly timer", timer);

    // set scene camera and add it to scene (to be updated)
    scene->setCamera(cam);
    scene->addObject(cam);

    // add some objects to scene
    scene->addObject(std::make_shared<MyBackground>(scene.get(), "bg"));
    scene->addObject(std::make_shared<MyObject>(scene.get(), "My Object", sf::Vector2f(-100.f, -100.f)));
    auto removedObject = std::make_shared<MyObject>(scene.get(), "Test Object", sf::Vector2f(100.f, 100.f));
    scene->addObject(removedObject);

    // set Test Object to be removed after 5 seconds
    removedObject->addTimer("makeTestObjectDead", &((new EGE::Timer(removedObject.get(), EGE::Timer::Mode::Limited, EGE::Time(5.0, EGE::Time::Unit::Seconds)))
                                     ->setCallback([scene, removedObject](std::string, EGE::Timer* timer) {
                                                   removedObject->setDead();
                                                })), EGE::GameLoop::TimerImmediateStart::Yes);

    // assign scene to GUI
    gui->addWidget(scene);

    // assign an instance of MyResourceManager to game loop
    gameLoop.setResourceManager(std::make_shared<MyResourceManager>());

    // assign GUI to loop
    gameLoop.setCurrentGUIScreen(gui);

    // run loop
    gameLoop.run();
}
RUN_TESTS(scene);
