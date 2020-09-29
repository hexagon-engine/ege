#include <testsuite/Tests.h>
#include <ege/scene/Scene2D.h>
#include <ege/scene/SceneObject2D.h>
#include <ege/scene/SceneWidget.h>
#include <ege/scene/TexturedRenderer2D.h>
#include <ege/gui/Animation.h>
#include <ege/gui/AnimationEasingFunctions.h>
#include <ege/gui/GUIGameLoop.h>
#include <ege/gui/Label.h>

// my object definition
class MyObject : public EGE::SceneObject2D
{
    std::shared_ptr<sf::Font> m_font;

public:
    MyObject(std::shared_ptr<EGE::Scene> owner, std::string name, sf::Vector2f pos)
    : EGE::SceneObject2D(owner, name), m_initialPosition(pos)
    {
        // set position from constructor
        setPosition(pos);

        // make object animated (it can be done now by flyTo())
        auto anim = make<EGE::Animation>(this, EGE::Time(1.0, EGE::Time::Unit::Seconds), EGE::Timer::Mode::Infinite);
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
    MyBackground(std::shared_ptr<EGE::Scene> owner, std::string name)
    : EGE::SceneObject2D(owner, name) {}

    void render(sf::RenderTarget& target) const
    {
        sf::VertexArray varr(sf::Quads, 4);
        varr.append(sf::Vertex(sf::Vector2f(getPosition().x-100.f, getPosition().y-100.f), sf::Color::Red));
        varr.append(sf::Vertex(sf::Vector2f(getPosition().x+100.f, getPosition().y-100.f), sf::Color::Green));
        varr.append(sf::Vertex(sf::Vector2f(getPosition().x+100.f, getPosition().y+100.f), sf::Color::Blue));
        varr.append(sf::Vertex(sf::Vector2f(getPosition().x-100.f, getPosition().y+100.f), sf::Color::Yellow));
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
        success &= (bool)loadTextureFromFile("texture.png").get();

        // return true if setDefaultFont succeeded or false if not (e.g. font doesn't exist)
        return success;
    }
};

TESTCASE(simple)
{
    // create loop and window
    EGE::GUIGameLoop gameLoop;
    gameLoop.setWindow(make<EGE::SFMLSystemWindow>(sf::VideoMode(300, 300), "EGE Scene Test"));

    // limit window framerate to 10
    gameLoop.getWindow().lock()->setFramerateLimit(60);

    // create main GUI
    auto gui = make<EGE::GUIScreen>(&gameLoop);

    // create scene
    auto scene = make<EGE::Scene>(&gameLoop);

    // add some objects to scene
    scene->addObject(make<MyObject>(scene, "My Object", sf::Vector2f(200.f, 200.f)));
    auto removedObject = make<MyObject>(scene, "Test Object", sf::Vector2f(100.f, 100.f));
    scene->addObject(removedObject);

    // set Test Object to be removed after 5 seconds
    removedObject->addTimer("makeTestObjectDead", &((new EGE::Timer(removedObject.get(), EGE::Timer::Mode::Limited, EGE::Time(5.0, EGE::Time::Unit::Seconds)))
                                     ->setCallback([scene, removedObject](std::string, EGE::Timer*) {
                                                   removedObject->setDead();
                                                })), EGE::GameLoop::TimerImmediateStart::Yes);

    // assign scene to GUI
    gui->addWidget(make<EGE::SceneWidget>(gui.get(), scene));

    // assign an instance of MyResourceManager to game loop
    gameLoop.setResourceManager(make<MyResourceManager>());

    // assign GUI to loop
    gameLoop.setCurrentGUIScreen(gui);

    // run loop
    gameLoop.run();
}

TESTCASE(_2dCamera)
{
    // create loop and window
    EGE::GUIGameLoop gameLoop;
    gameLoop.setWindow(make<EGE::SFMLSystemWindow>(sf::VideoMode(300, 300), "EGE Scene Test"));

    // limit window framerate to 10
    gameLoop.getWindow().lock()->setFramerateLimit(60);

    // create main GUI
    auto gui = make<EGE::GUIScreen>(&gameLoop);

    // create scene
    auto scene = make<EGE::Scene2D>(&gameLoop);

    // add camera
    auto cam = make<EGE::CameraObject2D>(scene);
    bool b1 = false;

    // make camera animated
    auto timer = make<EGE::Timer>(cam.get(), EGE::Timer::Mode::Infinite, EGE::Time(2.0, EGE::Time::Unit::Seconds));
    timer->setCallback([cam, &b1](std::string, EGE::Timer*) {
                        cam->flyTo(b1 ? sf::Vector2f(0.f, 100.f) : sf::Vector2f(0.f, -100.f), 1.0);
                        b1 = !b1;
                       });
    // the first fly
    cam->flyTo(sf::Vector2f(0.f, 100.f), 1.0);
    cam->addTimer("camera fly timer", timer);
    cam->setScalingMode(EGE::ScalingMode::Centered);

    // set scene camera and add it to scene (to be updated)
    scene->setCamera(cam);
    scene->addObject(cam);

    // add some objects to scene
    scene->addObject(make<MyBackground>(scene, "bg"));
    scene->addObject(make<MyObject>(scene, "My Object", sf::Vector2f(-100.f, -100.f)));
    auto removedObject = make<MyObject>(scene, "Test Object", sf::Vector2f(100.f, 100.f));
    scene->addObject(removedObject);

    auto texturedObject = make<EGE::SceneObject2D>(scene, "Textured Object");
    auto renderer = make<EGE::TexturedRenderer2D>(scene);
    renderer->setTextureName("texture.png");
    renderer->center();
    texturedObject->setRenderer(renderer);

    texturedObject->setPosition(sf::Vector2f(100.f, 100.f));
    scene->addObject(texturedObject);

    // set Test Object to be removed after 5 seconds
    removedObject->addTimer("makeTestObjectDead", &((new EGE::Timer(removedObject.get(), EGE::Timer::Mode::Limited, EGE::Time(5.0, EGE::Time::Unit::Seconds)))
                                     ->setCallback([scene, removedObject](std::string, EGE::Timer*) {
                                                   removedObject->setDead();
                                                })), EGE::GameLoop::TimerImmediateStart::Yes);

    // assign scene to GUI
    gui->addWidget(make<EGE::SceneWidget>(gui.get(), scene));

    // assign an instance of MyResourceManager to game loop
    gameLoop.setResourceManager(make<MyResourceManager>());

    // assign GUI to loop
    gameLoop.setCurrentGUIScreen(gui);

    // run loop
    gameLoop.run();
}

TESTCASE(serializer)
{
    // create loop
    EGE::GUIGameLoop gameLoop;
    gameLoop.setWindow(make<EGE::SFMLSystemWindow>(sf::VideoMode(300, 300), "EGE Scene Serializer Test"));

    // limit window framerate to 10
    gameLoop.getWindow().lock()->setFramerateLimit(60);

    // create main GUI
    auto gui = make<EGE::GUIScreen>(&gameLoop);

    // create scene
    auto scene = make<EGE::Scene2D>(&gameLoop);

    // create some object
    auto myObject = make<MyBackground>(scene, "My Test");
    myObject->setPosition(sf::Vector2f(0.f, 0.f));

    // serialize object
    auto data = myObject->serialize();
    std::cerr << data->toString() << std::endl;

    // deserialize object and add result
    auto myObject2 = make<MyBackground>(scene, "My Object 5555");
    myObject2->setPosition(sf::Vector2f(-100.f, -100.f));
    myObject2->deserialize(data);
    scene->addObject(myObject2);

    // assign scene to GUI
    gui->addWidget(make<EGE::SceneWidget>(gui.get(), scene));

    // assign an instance of MyResourceManager to game loop
    gameLoop.setResourceManager(make<MyResourceManager>());

    // assign GUI to loop
    gameLoop.setCurrentGUIScreen(gui);

    gameLoop.run();
}

RUN_TESTS(scene);
