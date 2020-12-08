#include <testsuite/Tests.h>
#include <ege/debug/Logger.h>
#include <ege/gfx/Renderer.h>
#include <ege/gui/Animation.h>
#include <ege/gui/AnimationEasingFunctions.h>
#include <ege/gui/GUIGameLoop.h>
#include <ege/gui/Label.h>
#include <ege/scene/ParticleSystem2D.h>
#include <ege/scene/Scene2D.h>
#include <ege/scene/SceneObject2D.h>
#include <ege/scene/SceneWidget.h>
#include <ege/scene/TexturedRenderer2D.h>
#include <ege/scene/TilemapRenderer2D.h>
#include <ege/tilemap/ChunkedTileMap2D.h>
#include <ege/tilemap/FixedTileMap2D.h>

// my object definition
class MyObject : public EGE::SceneObject2D
{
    std::shared_ptr<sf::Font> m_font;

public:
    MyObject(std::shared_ptr<EGE::Scene> owner, std::string name, sf::Vector2f pos)
    : EGE::SceneObject2D(owner, "MyObject"), m_initialPosition(pos)
    {
        setPosition(pos);
        setName(name);

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

    void render(sf::RenderTarget& target, const EGE::RenderStates& states) const
    {
        // render "low-level" SFML text centered to object position
        sf::Text text("MyObject: " + getName(), *m_font, 18);
        text.setPosition(getPosition().x, getPosition().y);
        text.setOrigin((int)text.getLocalBounds().width / 2, (int)text.getLocalBounds().height / 2);
        target.draw(text, states.sfStates());
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

    void render(sf::RenderTarget& target, const EGE::RenderStates& states) const
    {
        // add our 'test' shader
        EGE::RenderStates myStates = states;
        auto shader = getOwner()->getLoop()->getResourceManager().lock()->getShader("test");
        double disturb1 = (float)std::sin(getOwner()->getLoop()->time(EGE::Time::Unit::Seconds) * 5.14);
        double disturb2 = (float)std::sin(getOwner()->getLoop()->time(EGE::Time::Unit::Seconds) * 1.14);
        shader->setUniform("disturb1", (float)disturb1);
        shader->setUniform("disturb2", (float)disturb2);
        myStates.sfStates().shader = shader.get();

        sf::VertexArray varr(sf::Quads, 4);
        varr.append(sf::Vertex(sf::Vector2f(getPosition().x-100.f, getPosition().y-100.f), sf::Color::Red));
        varr.append(sf::Vertex(sf::Vector2f(getPosition().x+100.f, getPosition().y-100.f), sf::Color::Green));
        varr.append(sf::Vertex(sf::Vector2f(getPosition().x+100.f, getPosition().y+100.f), sf::Color::Blue));
        varr.append(sf::Vertex(sf::Vector2f(getPosition().x-100.f, getPosition().y+100.f), sf::Color::Yellow));
        target.draw(varr, myStates.sfStates());
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
        success &= (bool)loadShaderFromFile("test", "test.vert", "test.frag").get();

        // return true if all resources successfully loaded
        return success;
    }
};

TESTCASE(simple)
{
    // create loop and window
    EGE::GUIGameLoop gameLoop;
    gameLoop.openWindow(sf::VideoMode(300, 300), "EGE Scene Test");

    // limit window framerate to 10
    gameLoop.getWindow().setFramerateLimit(60);

    // create main GUI
    auto gui = make<EGE::GUIScreen>(gameLoop);

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
    gui->addWidget(make<EGE::SceneWidget>(*gui, scene));

    // assign an instance of MyResourceManager to game loop
    gameLoop.setResourceManager(make<MyResourceManager>());

    // assign GUI to loop
    gameLoop.setCurrentGUIScreen(gui);

    // run loop
    return gameLoop.run();
}

TESTCASE(_2dCamera)
{
    // create loop and window
    EGE::GUIGameLoop gameLoop;
    gameLoop.openWindow(sf::VideoMode(300, 300), "EGE Scene Test");

    // limit window framerate to 10
    gameLoop.getWindow().setFramerateLimit(60);

    // create main GUI
    auto gui = make<EGE::GUIScreen>(gameLoop);

    // create scene
    auto scene = make<EGE::Scene2D>(&gameLoop);

    // add camera
    auto cam = make<EGE::CameraObject2D>(scene);
    bool b1 = false;

    // set scaling mode
    cam->setScalingMode(EGE::ScalingMode::Centered);

    // make camera animated
    auto timer = make<EGE::Timer>(cam.get(), EGE::Timer::Mode::Infinite, EGE::Time(2.0, EGE::Time::Unit::Seconds));
    timer->setCallback([cam, &b1](std::string, EGE::Timer*) {
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
    gui->addWidget(make<EGE::SceneWidget>(*gui, scene));

    // assign an instance of MyResourceManager to game loop
    gameLoop.setResourceManager(make<MyResourceManager>());

    // assign GUI to loop
    gameLoop.setCurrentGUIScreen(gui);

    // run loop
    return gameLoop.run();
}

TESTCASE(serializer)
{
    // create loop
    EGE::GUIGameLoop gameLoop;
    gameLoop.openWindow(sf::VideoMode(300, 300), "EGE Scene Serializer Test");

    // limit window framerate to 10
    gameLoop.getWindow().setFramerateLimit(60);

    // create main GUI
    auto gui = make<EGE::GUIScreen>(gameLoop);

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
    gui->addWidget(make<EGE::SceneWidget>(*gui, scene));

    // assign an instance of MyResourceManager to game loop
    gameLoop.setResourceManager(make<MyResourceManager>());

    // assign GUI to loop
    gameLoop.setCurrentGUIScreen(gui);

    return gameLoop.run();
}

struct ParticleData : public EGE::ParticleSystem2D::UserData
{
    float motionx = 0.f, motiony = 0.5f;
    float color = 1.f;
    float ccp = 200.f;
};

class MyScene : public EGE::Scene2D
{
public:
    explicit MyScene(EGE::GUIGameLoop* loop)
    : EGE::Scene2D(loop) {}

    virtual void onUpdate(long long tickCounter)
    {
        // Update another objects
        EGE::Scene2D::onUpdate(tickCounter);

        // Update wind
        wind = std::sin((tickCounter + rand() % 100 / 100.f) / 100.f) * 100.f;
    }

    float wind = 0.f;
};

TESTCASE(particleSystem)
{
    // create loop
    EGE::GUIGameLoop loop;
    loop.openWindow(sf::VideoMode(600, 600), "Particle System");
    loop.setMinimalTickTime(EGE::Time(1 / 60.0, EGE::Time::Unit::Seconds));

    EGE::Renderer& renderer = loop.getRenderer();

    // create scene
    std::shared_ptr<MyScene> scene = make<MyScene>(&loop);

    // add wind speed variable
    //float wind = 0.f;

    // create particle system
    std::shared_ptr<EGE::ParticleSystem2D> particleSystem = make<EGE::ParticleSystem2D>(scene, sf::FloatRect(10.f, 10.f, 580.f, 1.f));
    particleSystem->setSpawnChance(50.0);
    particleSystem->setParticleLifeTime(400);
    particleSystem->setParticleUpdater([](EGE::ParticleSystem2D::Particle& particle) {
        ParticleData* myData = (ParticleData*)particle.userData.get();
        float meltFactor = (1.f - myData->color / 1.1f);

        // Gravity
        myData->motiony += 0.05f * meltFactor;
        particle.position.y += myData->motiony;
        if(particle.position.y > myData->ccp && myData->color > 0.f)
        {
            myData->color -= (rand() % 100) / 2500.f + 0.005f;
            if(myData->color < 0.f)
                myData->color = 0.f;
        }

        // Wind
        float wind = ((MyScene*)particle.system->getOwner().get())->wind;
        myData->motionx = wind / (particle.position.y + 0.5f) * meltFactor;
        particle.position.x += myData->motionx;
    });

    particleSystem->setParticleRenderer([&renderer](const std::list<EGE::ParticleSystem2D::Particle>& particles, sf::RenderTarget&, const EGE::RenderStates&) {
        log(EGE::LogLevel::Debug) << "Particles: " << particles.size();

        // Generate vertexes.
        std::vector<EGE::Vertex> vertexes;
        for(const EGE::ParticleSystem2D::Particle& particle: particles)
        {
            ParticleData* myData = (ParticleData*)particle.userData.get();
            float clf = (myData->color + 4.f) / 5.2f;
            sf::Color color(clf * 255, clf * 255, 255);
            vertexes.push_back(EGE::Vertex::make(EGE::Vec3d(particle.position.x, particle.position.y, 0.0), color));
            vertexes.push_back(EGE::Vertex::make(EGE::Vec3d(particle.position.x + myData->motionx, particle.position.y + myData->motiony, 0.0), color));
        }

        // Actually render them.
        renderer.renderPrimitives(vertexes, sf::Lines);
    });
    particleSystem->setParticleOnSpawn([](EGE::ParticleSystem2D::Particle& particle) {
        // Create user data instance.
        particle.userData = std::make_unique<ParticleData>();
        ParticleData* myData = (ParticleData*)particle.userData.get();

        // Randomize "melt" position.
        myData->ccp = rand() % 100 + 150.f;
    });

    // assign particle system to scene
    scene->addObject(particleSystem);

    // add camera
    auto cam = make<EGE::CameraObject2D>(scene);
    cam->setScalingMode(EGE::ScalingMode::None);
    scene->setCamera(cam);
    scene->addObject(cam);

    // create GUI
    std::shared_ptr<EGE::GUIScreen> gui = make<EGE::GUIScreen>(loop);
    gui->addWidget(make<EGE::SceneWidget>(*gui, scene));

    // assign GUI to loop
    loop.setCurrentGUIScreen(gui);

    // run game
    return loop.run();
}

struct MyTile
{
    int apx = 0, apy = 1;
};

class MyTileMapObject : public EGE::SceneObject2D
{
public:
    MyTileMapObject(std::shared_ptr<EGE::Scene> owner)
    : EGE::SceneObject2D(owner, "mytilemapobject")
    {
        m_tilemap = make<EGE::ChunkedTileMap2D<MyTile, 4, 4>>();
        //m_tilemap->initialize({0, 1}); // water
        m_tilemap->setTileSize({64, 64});

        // Pre-generate some chunks
        m_tilemap->regenerateChunk({-1, -1});
        m_tilemap->regenerateChunk({0, 0});
        m_tilemap->regenerateChunk({1, 1});
        m_tilemap->regenerateChunk({2, 2});
        m_tilemap->regenerateChunk({3, 2});
        m_tilemap->regenerateChunk({4, 3});

        if(!owner->isHeadless())
        {
            typedef EGE::TilemapRenderer2D<decltype(m_tilemap)::element_type> RendererType;
            auto renderer = make<RendererType>(owner, m_tilemap);
            renderer->setAtlasTextureName("atlas.png");
            renderer->setTileAtlasMapper( [](const MyTile& tile, EGE::Vector2<EGE::MaxInt> tilePos, EGE::Size, RendererType::AtlasInfo& info) {
                info.texCoords = EGE::Vec2i(tile.apx * 64, tile.apy * 64);
                info.rotation = tilePos.x;
            });
            setRenderer(renderer);
        }
    }
private:
    std::shared_ptr<EGE::ChunkedTileMap2D<MyTile, 4, 4>> m_tilemap;
};

class MyResourceManager2 : public EGE::ResourceManager
{
public:
    virtual bool reload() override
    {
        bool success = true;
        success &= (bool)loadTextureFromFile("atlas.png");
        return success;
    }
};

TESTCASE(_tileMap)
{
    EGE::GUIGameLoop loop;
    loop.openWindow(sf::VideoMode(600, 600), "Tile Map");
    loop.setMinimalTickTime(EGE::Time(1 / 60.0, EGE::Time::Unit::Seconds));
    loop.setResourceManager(make<MyResourceManager2>());

    auto scene = make<EGE::Scene2D>(&loop);
    scene->addObject(make<MyTileMapObject>(scene));

    auto camera = make<EGE::CameraObject2D>(scene);
    camera->setPosition({512, 512});
    camera->setScalingMode(EGE::ScalingMode::Centered);
    scene->addObject(camera);
    scene->setCamera(camera);

    auto gui = make<EGE::GUIScreen>(loop);
    gui->addWidget(make<EGE::SceneWidget>(*gui, scene));
    loop.setCurrentGUIScreen(gui);

    return loop.run();
}

RUN_TESTS(scene);
