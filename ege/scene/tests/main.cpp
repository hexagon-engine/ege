#include <testsuite/Tests.h>
#include <ege/debug/Logger.h>
#include <ege/gfx/Renderer.h>
#include <ege/gui/Animation.h>
#include <ege/gui/AnimationEasingFunctions.h>
#include <ege/gui/GUIGameLoop.h>
#include <ege/gui/Label.h>
#include <ege/scene/ParticleSystem2D.h>
#include <ege/scene/Scene2D.h>
#include <ege/scene/SceneLoader.h>
#include <ege/scene/SceneObject2D.h>
#include <ege/scene/SceneWidget.h>
#include <ege/scene/TexturedRenderer2D.h>
#include <ege/scene/TilemapRenderer2D.h>
#include <ege/tilemap/ChunkedTileMap2D.h>
#include <ege/tilemap/FixedTileMap2D.h>
#include <ege/util/system.h>

// my object definition
class MyObject : public EGE::SceneObject2D
{
    std::shared_ptr<sf::Font> m_font;

public:
    // Objects registered in Scene Object Creator must have its "empty" state!
    MyObject(EGE::Scene2D& owner, std::string name = "To Be Serialized", EGE::Vec2d pos = {})
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
                        setPosition(m_initialPosition + EGE::Vec2d(val * 20.f, 0));
                     });
    }

    void updateGeometry(EGE::Renderer&) override
    {
        if(!m_font)
        {
            m_font = m_owner.getLoop()->getResourceManager()->getDefaultFont();
            ASSERT(m_font);
        }
    }

    void render(EGE::Renderer& renderer) const override
    {
        EGE::Renderer::TextWithBackgroundSettings settings;
        settings.font_size = 18;
        settings.text_align = EGE::Renderer::TextAlign::Center;
        settings.color = sf::Color::White;
        settings.background_color = sf::Color::Transparent;
        renderer.renderTextWithBackground(getPosition().x, getPosition().y, *m_font, "MyObject: " + getName(), settings);
    }

    bool deserializeMain(EGE::SharedPtr<EGE::ObjectMap> data) override
    {
        m_initialPosition = EGE::Serializers::toVector2(data->getObject("ip").to<EGE::ObjectMap>().value());
        return EGE::SceneObject2D::deserializeMain(data);
    }

    EGE::SharedPtr<EGE::ObjectMap> serializeMain() const override
    {
        auto object = EGE::SceneObject2D::serializeMain();
        object->addObject("ip", EGE::Serializers::fromVector2(m_initialPosition));
        return object;
    }

    void setDead() { m_dead = true; }

private:
    EGE::Vec2d m_initialPosition;
};

class MyBackground : public EGE::SceneObject2D
{
public:
    MyBackground(EGE::Scene2D& owner, std::string name = "")
    : EGE::SceneObject2D(owner, "MyBackground") { setName(name); }

    void render(EGE::Renderer& renderer) const override
    {
        // add our 'test' shader
        EGE::RenderStates myStates = renderer.getStates();
        auto shader = getOwner().getLoop()->getResourceManager()->getShader("test");
        double disturb1 = (float)std::sin(getOwner().getLoop()->time(EGE::Time::Unit::Seconds) * 5.14);
        double disturb2 = (float)std::sin(getOwner().getLoop()->time(EGE::Time::Unit::Seconds) * 1.14);
        shader->setUniform("disturb1", (float)disturb1);
        shader->setUniform("disturb2", (float)disturb2);
        myStates.sfStates().shader = shader.get();

        sf::VertexArray varr(sf::Quads, 4);
        varr.append(sf::Vertex(sf::Vector2f(getPosition().x-100.f, getPosition().y-100.f), sf::Color::Red));
        varr.append(sf::Vertex(sf::Vector2f(getPosition().x+100.f, getPosition().y-100.f), sf::Color::Green));
        varr.append(sf::Vertex(sf::Vector2f(getPosition().x+100.f, getPosition().y+100.f), sf::Color::Blue));
        varr.append(sf::Vertex(sf::Vector2f(getPosition().x-100.f, getPosition().y+100.f), sf::Color::Yellow));
        renderer.getTarget().draw(varr, myStates.sfStates());
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
    auto scene = make<EGE::Scene2D>(&gameLoop);

    // add some objects to scene
    scene->addObject(make<MyObject>(*scene, "My Object", EGE::Vec2d(200.f, 200.f)));
    auto removedObject = make<MyObject>(*scene, "Test Object", EGE::Vec2d(100.f, 100.f));
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
    auto cam = make<EGE::CameraObject2D>(*scene);
    bool b1 = false;

    // set scaling mode
    cam->setScalingMode(EGE::ScalingMode::Centered);

    // make camera animated
    auto timer = make<EGE::Timer>(cam.get(), EGE::Timer::Mode::Infinite, EGE::Time(2.0, EGE::Time::Unit::Seconds));
    timer->setCallback([cam, &b1](std::string, EGE::Timer*) {
                        cam->flyTo(b1 ? EGE::Vec2d(0.f, 100.f) : EGE::Vec2d(0.f, -100.f), 1.0, EGE::AnimationEasingFunctions::easeInOutCubic);
                        b1 = !b1;
                       });
    // the first fly
    cam->flyTo(EGE::Vec2d(0.f, 100.f), 1.0, EGE::AnimationEasingFunctions::easeInOutCubic);
    cam->addTimer("camera fly timer", timer);

    // set scene camera and add it to scene (to be updated)
    scene->setCamera(cam);
    scene->addObject(cam);

    // add some objects to scene
    scene->addObject(make<MyBackground>(*scene, "bg"));
    scene->addObject(make<MyObject>(*scene, "My Object", EGE::Vec2d(-100.f, -100.f)));
    auto removedObject = make<MyObject>(*scene, "Test Object", EGE::Vec2d(100.f, 100.f));
    scene->addObject(removedObject);

    auto texturedObject = make<EGE::SceneObject2D>(*scene, "Textured Object");
    auto renderer = make<EGE::TexturedRenderer2D>(*texturedObject);
    renderer->setTextureName("texture.png");
    renderer->center();
    texturedObject->setRenderer(renderer);

    texturedObject->setPosition(EGE::Vec2d(100.f, 100.f));
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
    auto myObject = make<MyBackground>(*scene, "My Test");
    myObject->setPosition(EGE::Vec2d(0.f, 0.f));

    // serialize object
    auto data = myObject->serialize();
    std::cerr << data->toString() << std::endl;

    // deserialize object and add result
    auto myObject2 = make<MyBackground>(*scene, "My Object 5555");
    myObject2->setPosition(EGE::Vec2d(-100.f, -100.f));
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

    // create scene
    std::shared_ptr<MyScene> scene = make<MyScene>(&loop);

    // add wind speed variable
    //float wind = 0.f;

    // create particle system
    std::shared_ptr<EGE::ParticleSystem2D> particleSystem = make<EGE::ParticleSystem2D>(*scene, sf::FloatRect(10.f, 10.f, 580.f, 1.f));
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
        float wind = ((MyScene&)particle.system.getOwner()).wind;
        myData->motionx = wind / (particle.position.y + 0.5f) * meltFactor;
        particle.position.x += myData->motionx;
    });

    particleSystem->setParticleRenderer([](const std::list<EGE::ParticleSystem2D::Particle>& particles, EGE::Renderer& renderer) {
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
    auto cam = make<EGE::CameraObject2D>(*scene);
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
    MyTileMapObject(EGE::Scene2D& owner)
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

        if(!owner.isHeadless())
        {
            typedef EGE::TilemapRenderer2D<decltype(m_tilemap)::element_type> RendererType;
            auto renderer = make<RendererType>(*this, m_tilemap);
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
    scene->addObject(make<MyTileMapObject>(*scene));

    auto camera = make<EGE::CameraObject2D>(*scene);
    camera->setPosition({512, 512});
    camera->setScalingMode(EGE::ScalingMode::Centered);
    scene->addObject(camera);
    scene->setCamera(camera);

    auto gui = make<EGE::GUIScreen>(loop);
    gui->addWidget(make<EGE::SceneWidget>(*gui, scene));
    loop.setCurrentGUIScreen(gui);

    return loop.run();
}

TESTCASE(sceneLoader)
{
    // Create loop (it's needed for Scene
    EGE::GUIGameLoop loop;

    // Setup typeID registry
    EGE::SceneLoader::SceneObjectCreatorRegistry registry;
    registry.addEntry("MyObject", EGE_SCENE2D_OBJECT_CREATOR(MyObject));
    registry.addEntry("MyBackground", EGE_SCENE2D_OBJECT_CREATOR(MyBackground));

    // Load some scene
    auto scene = make<EGE::Scene2D>(&loop);

    EGE::SceneLoader loader(*scene, registry);
    if(!loader.loadSceneAndSave("saves/test.json", "res/scenes/test.json"))
    {
        log() << "Failed to load scene!";
        return 1;
    }

    // Add camera
    auto camera = make<EGE::CameraObject2D>(*scene);
    camera->setPosition({0, 0});
    camera->setScalingMode(EGE::ScalingMode::Centered);
    scene->addObject(camera);
    scene->setCamera(camera);

    // Change something in loaded static objects
    {
        auto so1 = scene->getStaticObject(2);
        so1->setName("SO1");
        auto so2 = scene->getStaticObject(3);
        so2->setName("SO2");
    }

    // Assign Resource Manager
    loop.setResourceManager(make<MyResourceManager>());

    // Open some window to see results.
    loop.openWindow(sf::VideoMode(600, 600), "SceneLoader");
    auto gui = make<EGE::GUIScreen>(loop);
    gui->addWidget(make<EGE::SceneWidget>(*gui, scene));
    loop.setCurrentGUIScreen(gui);

    if(loop.run() != 0)
        return 2;

    // Try save scene
    EGE::System::createDirectory("saves");
    if(!loader.saveScene("saves/test.json"))
        return 3;

    return 0;
}

TESTCASE(parenting)
{
    // Setup registry
    EGE::SceneLoader::SceneObjectCreatorRegistry registry;
    registry.addEntry("MyObject", EGE_SCENE2D_OBJECT_CREATOR(MyObject));
    registry.addEntry("MyBackground", EGE_SCENE2D_OBJECT_CREATOR(MyBackground));

    // Setup loop and load scene
    EGE::GUIGameLoop loop;
    auto scene = make<EGE::Scene2D>(&loop);
    if(!scene->loadFromFile("saves/parenting.json", "res/scenes/parenting.json", registry))
        return 1;

    // Add camera
    {
        auto camera = make<EGE::CameraObject2D>(*scene);
        camera->setPosition({0, 0});
        camera->setScalingMode(EGE::ScalingMode::Centered);
        camera->setParent(scene->getStaticObject(1).get());

        // Add some animation
        auto anim = make<EGE::Animation>(camera.get(), EGE::Time(1.4, EGE::Time::Unit::Seconds), EGE::Timer::Mode::Infinite);
        anim->addKeyframe(0, -1);
        anim->addKeyframe(0.25, 1);
        anim->addKeyframe(0.5, 0);
        anim->addKeyframe(0.75, 1);
        anim->addKeyframe(1, -1);
        anim->setEasingFunction(EGE::AnimationEasingFunctions::easeInOutSine);

        camera->addAnimation(anim, [](EGE::Animation* obj, double fac) {
            EGE::CameraObject2D* cam = (EGE::CameraObject2D*)obj->getLoop();
            cam->setPosition({0, fac * 50});
        });

        scene->addObject(camera);
        scene->setCamera(camera);
    }

    // Assign ResourceManager & open window & GUI
    loop.setResourceManager(make<MyResourceManager>());
    loop.openWindow(sf::VideoMode(600, 600), "Parenting");
    auto gui = make<EGE::GUIScreen>(loop);
    gui->addWidget(make<EGE::SceneWidget>(*gui, scene));
    loop.setCurrentGUIScreen(gui);

    // Run game
    loop.run();

    // Save scene
    if(!scene->saveToFile("saves/parenting.json", registry))
        return 2;
    return 0;
}

RUN_TESTS(scene);
