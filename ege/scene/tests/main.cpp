#include <testsuite/Tests.h>
#include <ege/scene/Scene.h>
#include <ege/gui/Animation.h>
#include <ege/gui/AnimationEasingFunctions.h>
#include <ege/gui/GUIGameLoop.h>
#include <ege/gui/Label.h>

// my object definition
class MyObject : public EGE::SceneObject
{
public:
    MyObject(EGE::Scene* owner, std::string name, sf::Vector2f pos)
    : EGE::SceneObject(owner, name), m_initialPosition(pos), m_position(pos)
    {
        // make object animated
        auto anim = std::make_shared<EGE::Animation>(this, EGE::Time(1.0, EGE::Time::Unit::Seconds), EGE::Timer::Mode::Infinite);
        anim->addKeyframe(0.0, -1.0);
        anim->addKeyframe(0.5, 1.0);
        anim->addKeyframe(1.0, -1.0);
        anim->setEasingFunction(EGE::AnimationEasingFunctions::easeInOutQuad);
        addAnimation(anim, [this](EGE::Animation*, double val) {
                        m_position = m_initialPosition + sf::Vector2f(val * 20.f, 0);
                     });
    }

    void render(sf::RenderTarget& target)
    {
        // Render my object as simple label displaying its name.
        // If we want to use EGE widgets inside this function, set
        // its parent to m_owner (not NULL).
        EGE::Label label(m_owner);
        label.setString("Object " + getName());
        label.setPosition(m_position);
        label.setTextAlign(EGE::Label::Align::Center);
        label.render(target);
    }
    void setDead()
    {
        m_dead = true;
    }

private:
    sf::Vector2f m_initialPosition;
    sf::Vector2f m_position;
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

    // add some object to GUI
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

RUN_TESTS(scene);
