#include <testsuite/Tests.h>
#include <ege/gui.h>
#include <ege/scene.h>

class Arm : public EGE::SceneObject
{
public:
    EGE_SCENEOBJECT("Arm")

    Arm(EGE::Scene& owner)
    : EGE::SceneObject(owner) {}

    virtual void onInit() override
    {
        auto animation = make<EGE::NumberAnimation>(*this, EGE::Time(5), EGE::Timer::Mode::Infinite);
        animation->addKeyframe(0, 0);
        animation->addKeyframe(1, 360);
        addAnimation<EGE::MaxFloat>(animation, [&](auto&, auto value) {
            setRotation(value);
        });
    }
};

class End : public Arm
{
public:
    EGE_SCENEOBJECT("End")

    End(EGE::Scene& owner)
    : Arm(owner) {}

    virtual void onInit() override
    {
        Arm::onInit();
        auto particleSystem = getOwner().addNewObject<EGE::ParticleSystem2D>();
        particleSystem->setSpawnRect({-0.1, -0.1, 0.2, 0.2});
        particleSystem->setParticleLifeTime(180);
        particleSystem->setParticleRenderer([](const auto& particles, EGE::Renderer& renderer) {
            sf::VertexArray varr(sf::Points, particles.size());
            size_t counter = 0;
            for(auto& particle: particles)
            {
                auto color = static_cast<sf::Uint8>(particle.ttl * 255 / 180);
                varr[counter] = sf::Vertex({static_cast<float>(particle.position.x), static_cast<float>(particle.position.y)}, {0, color, color});
                counter++;
            }
            renderer.getTarget().draw(varr);
        });
        particleSystem->setParent(this);
        particleSystem->setRenderLayer(-1);
    }
};

TESTCASE(basic)
{
    EGE::GUIGameLoop loop;

    auto registry = make<EGE::SceneObjectRegistry>();
    if(!registry->loadFromFile("objects/relativecoords.json"))
        return 1;

    registry->addType<Arm>();
    registry->addType<End>();

    auto scene = make<EGE::Scene>(&loop, registry);
    if(!scene->loadFromFile("", "scenes/relativecoords.json"))
        return 1;

    auto camera = scene->addNewObject<EGE::Plain2DCamera>();
    camera->setDisplaySize({340, 340});
    camera->setScalingMode(EGE::ScalingMode::Fit);
    scene->setCamera(camera);

    auto guiScreen = make<EGE::GUIScreen>(loop);
    guiScreen->addNewWidget<EGE::SceneWidget>(scene);
    loop.setCurrentGUIScreen(guiScreen);
    loop.openWindow(sf::VideoMode(340, 340), "Relative Coords");

    return loop.run();
}

RUN_TESTS(relativecoords)
