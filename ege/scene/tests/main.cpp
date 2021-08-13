#include <testsuite/Tests.h>
#include <ege/debug/Logger.h>
#include <ege/gfx/Renderer.h>
#include <ege/gui/Animation.h>
#include <ege/gui/AnimationEasingFunctions.h>
#include <ege/gui/GUIGameLoop.h>
#include <ege/gui/Label.h>
#include <ege/scene/ParticleSystem2D.h>
#include <ege/scene/Scene.h>
#include <ege/scene/SceneLoader.h>
#include <ege/scene/SceneWidget.h>
#include <ege/scene/Plain2DCamera.h>
#include <ege/tilemap/ChunkedTileMap2D.h>
#include <ege/tilemap/FixedTileMap2D.h>
#include <ege/util/system.h>

// my object definition
class MyObject : public EGE::SceneObject
{
    EGE::SharedPtr<sf::Font> m_font;

public:
    EGE_SCENEOBJECT("MyObject");

    // Objects registered in Scene Object Creator must have its "empty" state!
    MyObject(EGE::Scene& owner)
    : EGE::SceneObject(owner) {}

    virtual void onInit() override
    {
        // make object animated (it can be done now by flyTo())
        auto anim = make<EGE::Vec2Animation>(*this, 1.0, EGE::Timer::Mode::Infinite);
        anim->addKeyframe(0.0, getPosition().toVec2d() - EGE::Vec2d(20.0, 20.0));
        anim->addKeyframe(0.5, getPosition().toVec2d() + EGE::Vec2d(20.0, 20.0));
        anim->addKeyframe(1.0, getPosition().toVec2d() - EGE::Vec2d(20.0, 20.0));
        anim->setEasingFunction(EGE::AnimationEasingFunctions::easeInOutQuad);
        addAnimation<EGE::Vec2d>(anim, [this](EGE::Vec2Animation&, EGE::Vec2d val) {
            setPosition(val);
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
        settings.color = EGE::Colors::white;
        settings.background_color = EGE::Colors::transparent;
        renderer.renderTextWithBackground(getPosition().x, getPosition().y, *m_font, "MyObject: " + getName(), settings);
    }

    EGE::SharedPtr<EGE::ObjectMap> serializeMain() const override
    {
        auto object = EGE::SceneObject::serializeMain();
        object->addObject("ip", EGE::Serializers::fromVector2(m_initialPosition));
        return object;
    }

    void setDead() { m_dead = true; }

private:
    EGE::Vec2d m_initialPosition;
};

class MyBackground : public EGE::SceneObject
{
public:
    EGE_SCENEOBJECT("MyBackground");

    MyBackground(EGE::Scene& owner)
    : EGE::SceneObject(owner)
    {
        auto anim = make<EGE::RGBAnimation>(*this, 5.0, EGE::Timer::Mode::Infinite);
        anim->addKeyframe(0.0, EGE::Colors::red);
        anim->addKeyframe(0.25, EGE::Colors::cyan);
        anim->addKeyframe(0.5, EGE::Colors::magenta);
        anim->addKeyframe(0.75, EGE::Colors::white);
        anim->addKeyframe(1.0, EGE::Colors::red);
        anim->setEasingFunction(EGE::AnimationEasingFunctions::easeInOutQuad);
        addAnimation<EGE::ColorRGBA>(anim, [this](EGE::RGBAnimation&, EGE::ColorRGBA val) {
            m_color = val;
        });
    }
    void render(EGE::Renderer& renderer) const override
    {
        // add our 'test' shader
        EGE::RenderStates myStates = renderer.getStates();
        /*auto shader = getOwner().getLoop()->getResourceManager()->getShader("test");
        double disturb1 = (float)std::sin(getOwner().getLoop()->time(EGE::Time::Unit::Seconds) * 5.14);
        double disturb2 = (float)std::sin(getOwner().getLoop()->time(EGE::Time::Unit::Seconds) * 1.14);
        shader->setUniform("disturb1", (float)disturb1);
        shader->setUniform("disturb2", (float)disturb2);
        myStates.sfStates().shader = shader.get();*/

        sf::VertexArray varr(sf::Quads, 4);
        varr.append(sf::Vertex(sf::Vector2f(getPosition().x-100.f, getPosition().y-100.f), sf::Color(m_color.r * 255, m_color.g * 255, m_color.b * 255, m_color.a * 255)));
        varr.append(sf::Vertex(sf::Vector2f(getPosition().x+100.f, getPosition().y-100.f), sf::Color::Green));
        varr.append(sf::Vertex(sf::Vector2f(getPosition().x+100.f, getPosition().y+100.f), sf::Color::Blue));
        varr.append(sf::Vertex(sf::Vector2f(getPosition().x-100.f, getPosition().y+100.f), sf::Color::Yellow));
        renderer.getTarget().draw(varr, myStates.sfStates());
    }

    EGE::ColorRGBA m_color;
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
        success &= (bool)loadTextureFromFile("texturedPart.png").get();
        success &= (bool)loadShaderFromFile("test", "test.vert", "test.frag").get();

        // return true if all resources successfully loaded
        return success;
    }
};

TESTCASE(serializer)
{
    // create loop
    EGE::GUIGameLoop gameLoop;
    auto window = gameLoop.openWindow(sf::VideoMode(300, 300), "EGE Scene Serializer Test");

    // limit window framerate to 10
    window->setFramerateLimit(60);

    // create main GUI
    auto gui = window->setNewGUIScreen<EGE::GUIScreen>();

    // create scene
    auto scene = make<EGE::Scene>(gameLoop);

    // add object types
    auto& registry = scene->getRegistry();
    registry.addType<MyBackground>();

    // create some object
    auto myObject = scene->createObject<MyBackground>(nullptr);
    myObject->setName("My Test");
    myObject->setPosition(EGE::Vec2d(0.f, 0.f));

    // serialize object
    auto data = myObject->serialize();
    std::cerr << data->toString() << std::endl;

    // deserialize object and add result
    auto myObject2 = scene->addNewObject<MyBackground>(data);
    myObject2->setName("My Object 5555");
    myObject2->setPosition(EGE::Vec2d(-100.f, -100.f));

    // assign scene to GUI
    gui->addNewWidget<EGE::SceneWidget>(scene);

    // assign an instance of MyResourceManager to game loop
    gameLoop.setResourceManager(make<MyResourceManager>());

    return gameLoop.run();
}

struct MyParticle : public EGE::Particle
{
    float motionx = 0.f, motiony = 0.5f;
    float color = 1.f;
    float ccp = 200.f;
};

class MyScene : public EGE::Scene
{
public:
    explicit MyScene(EGE::GUIGameLoop& loop)
    : EGE::Scene(loop) {}

    virtual void onTick()
    {
        // Update another objects
        EGE::Scene::onTick();

        // Update wind
        wind = std::sin((getTickCount() + rand() % 100 / 100.f) / 100.f) * 100.f;
    }

    float wind = 0.f;
};

class MyParticleSystem : public EGE::ParticleSystem2D<MyParticle>
{
public:
    EGE_SCENEOBJECT("MyParticleSystem")

    MyParticleSystem(EGE::Scene& owner)
    : EGE::ParticleSystem2D<MyParticle>(owner) {}

private:
    virtual void onParticleUpdate(ParticleType&) const override;
    virtual void onParticleSpawn(ParticleType&) const override;
    virtual void renderParticles(const std::list<ParticleType>&, EGE::Renderer&) const override;
};

void MyParticleSystem::onParticleUpdate(ParticleType& particle) const
{
    float meltFactor = (1.f - particle.color / 1.1f);

    // Gravity
    particle.motiony += 0.05f * meltFactor;
    particle.position.y += particle.motiony;
    if(particle.position.y > particle.ccp && particle.color > 0.f)
    {
        particle.color -= (rand() % 100) / 2500.f + 0.005f;
        if(particle.color < 0.f)
            particle.color = 0.f;
    }

    // Wind
    float wind = ((MyScene&)getOwner()).wind;
    particle.motionx = wind / (particle.position.y + 0.5f) * meltFactor;
    particle.position.x += particle.motionx;
}
void MyParticleSystem::onParticleSpawn(ParticleType& particle) const
{
    // Randomize "melt" position.
    particle.ccp = rand() % 100 + 150.f;
}
void MyParticleSystem::renderParticles(const std::list<ParticleType>& particles, EGE::Renderer& renderer) const
{
    ege_log.debug() << "Particles: " << particles.size();

    // Generate vertexes.
    sf::VertexArray varr(sf::Lines, particles.size() * 2);
    size_t counter = 0;
    for(auto& particle: particles)
    {
        float clf = (particle.color + 4.f) / 5.2f;
        sf::Color color(clf * 255, clf * 255, 255);
        varr[counter * 2] =     {{static_cast<float>(particle.position.x), static_cast<float>(particle.position.y)}, color};
        varr[counter * 2 + 1] = {{static_cast<float>(particle.position.x + particle.motionx), static_cast<float>(particle.position.y + particle.motiony)}, color};
        counter++;
    }

    // Actually render them.
    renderer.getTarget().draw(varr, renderer.getStates().sfStates());
}

TESTCASE(particleSystem)
{
    // create loop
    EGE::GUIGameLoop loop;
    auto window = loop.openWindow(sf::VideoMode(600, 600), "Particle System");
    loop.setMinimalTickTime(EGE::Time(1 / 60.0, EGE::Time::Unit::Seconds));

    // create scene
    EGE::SharedPtr<MyScene> scene = make<MyScene>(loop);
    auto& registry = scene->getRegistry();
    registry.addType<MyParticleSystem>();

    // add wind speed variable
    //float wind = 0.f;

    // create particle system
    EGE::SharedPtr<MyParticleSystem> particleSystem = scene->addNewObject<MyParticleSystem>();
    particleSystem->setSpawnRect({10.f, 10.f, 580.f, 1.f});
    particleSystem->setSpawnChance(50.0);
    particleSystem->setParticleLifeTime(400);

    // add camera
    auto cam = scene->addNewObject<EGE::Plain2DCamera>();
    cam->setScalingMode(EGE::ScalingMode::None);

    // create GUI
    auto gui = window->setNewGUIScreen<EGE::GUIScreen>();
    auto sceneWidget = gui->addNewWidget<EGE::SceneWidget>(scene);
    sceneWidget->setCamera(cam);

    // run game
    return loop.run();
}

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

TESTCASE(sceneLoader)
{
    // Create loop (it's needed for Scene)
    EGE::GUIGameLoop loop;

    // Load some scene
    auto scene = make<EGE::Scene>(loop);

    EGE::SceneObjectRegistry& registry = scene->getRegistry();
    registry.addType<MyObject>();
    registry.addType<MyBackground>();

    EGE::SceneLoader loader(*scene);
    if(!loader.loadSceneAndSave("test.json", "scenes/test.json"))
    {
        ege_log.error() << "Failed to load scene!";
        return 1;
    }

    // Change something in loaded static objects
    {
        auto so1 = scene->getObjectByName("2");
        so1->setName("S__O1");
        auto so2 = scene->getObjectByName("3");
        so2->setName("S__O2");
    }

    // Assign Resource Manager
    loop.setResourceManager(make<MyResourceManager>());

    // Open some window to see results.
    auto window = loop.openWindow(sf::VideoMode(600, 600), "SceneLoader");
    auto gui = window->setNewGUIScreen<EGE::GUIScreen>();
    auto sceneWidget = gui->addNewWidget<EGE::SceneWidget>(scene);

    // Add camera
    auto camera = scene->addNewObject<EGE::Plain2DCamera>(nullptr);
    camera->setPosition({0, 0});
    camera->setScalingMode(EGE::ScalingMode::Centered);
    sceneWidget->setCamera(camera);

    if(loop.run() != 0)
        return 2;

    // Try save scene
    EGE::System::createDirectory("saves");
    if(!loader.saveScene("test.json"))
        return 3;

    return 0;
}

class SimpleRectangleObject : public EGE::SceneObject
{
public:
    EGE_SCENEOBJECT("SimpleRectangleObject");

    SimpleRectangleObject(EGE::Scene& scene)
    : EGE::SceneObject(scene)
    {
        auto anim = make<EGE::Vec2Animation>(*this, 1.0, EGE::Timer::Mode::Infinite);
        anim->addKeyframe(0.0, m_initialPosition - EGE::Vec2d(10.0, 0));
        anim->addKeyframe(0.5, m_initialPosition + EGE::Vec2d(10.0, 0));
        anim->addKeyframe(1.0, m_initialPosition - EGE::Vec2d(10.0, 0));
        anim->setEasingFunction(EGE::AnimationEasingFunctions::easeInOutQuad);
        addAnimation<EGE::Vec2d>(anim, [this](EGE::Vec2Animation&, EGE::Vec2d val) {
            setPosition(val);
        });
    }

    virtual void render(EGE::Renderer& renderer) const
    {
        sf::RectangleShape rs(sf::Vector2f(20, 20));
        rs.setOrigin(10, 10);
        auto pos = getPosition();
        rs.setPosition({(float)pos.x, (float)pos.y});
        rs.setRotation(getRotation());
        renderer.getTarget().draw(rs);
    }

    virtual void onTick()
    {
        EGE::SceneObject::onTick();
        if(!m_parent)
            setRotation(getRotation() + 0.01);
    }

    EGE::Vec2d m_initialPosition;
};

TESTCASE(parenting)
{
    // Setup loop and load scene
    EGE::GUIGameLoop loop;
    auto scene = make<EGE::Scene>(loop);

    // Load other objects
    EGE::SceneObjectRegistry& registry = scene->getRegistry();
    if(!registry.loadFromFile("objects/registry.json"))
        return 3;

    registry.addType<MyObject>();
    registry.addType<MyBackground>();
    registry.addType<SimpleRectangleObject>();

    if(!scene->loadFromFile("parenting.json", "scenes/parenting.json"))
        return 1;

    // Assign ResourceManager
    loop.setResourceManager(make<MyResourceManager>());

    // Add window, GUI and camera
    {
        auto window = loop.openWindow(sf::VideoMode(600, 600), "Parenting");
        auto gui = window->setNewGUIScreen<EGE::GUIScreen>();
        auto camera = scene->addNewObject<EGE::Plain2DCamera>();
        camera->setPosition({0, 0});
        camera->setScalingMode(EGE::ScalingMode::Centered);
        auto sceneWidget = gui->addNewWidget<EGE::SceneWidget>(scene);
        sceneWidget->setCamera(camera);
    }

    // Run game
    loop.run();

    // Save scene
    if(!scene->saveToFile("parenting.json"))
        return 2;
    return 0;
}

RUN_TESTS(scene);
