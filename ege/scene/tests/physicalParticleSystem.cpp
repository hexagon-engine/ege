#include <testsuite/Tests.h>
#include <ege/gui.h>
#include <ege/scene.h>
#include <ege/util.h>

class MyParticle : public EGE::PhysicalParticle
{
public:
    EGE::Uint8 color;
};

class MyParticleSystem : public EGE::PhysicalParticleSystem<MyParticle>
{
public:
    EGE_SCENEOBJECT("MyParticleSystem")

    MyParticleSystem(EGE::Scene& scene)
    : EGE::PhysicalParticleSystem<MyParticle>(scene)
    {
        setGravity({0, 0.0002, 0});
        setStartMotion({0, -0.15, 0});
        setStartMotionAngleRandom(18);
        setStartMotionValueRandom(0.3);
        setSpawnRect(EGE::RectD(4, 10).center());
        setParticleLifeTime(2400);
    }

    virtual void onParticleSpawn(MyParticle& particle) const override
    {
        EGE::PhysicalParticleSystem<MyParticle>::onParticleSpawn(particle);
        particle.color = EGE::randomDouble(25, 50);
    }

    virtual void renderParticles(const std::list<MyParticle>& particles, EGE::Renderer& renderer) const override
    {
        sf::VertexArray varr(sf::Lines, particles.size() * 2);
        size_t counter = 0;
        for(auto& particle: particles)
        {
            auto color = particle.ttl * 255 / m_particleTTL;
            varr[2*counter] = sf::Vertex({static_cast<float>(particle.position.x-particle.motion.x*10), static_cast<float>(particle.position.y-particle.motion.y*10)},
                {static_cast<sf::Uint8>(color), static_cast<EGE::Uint8>(color/2), 0}
            );
            varr[2*counter+1] = sf::Vertex({static_cast<float>(particle.position.x), static_cast<float>(particle.position.y)},
                {static_cast<sf::Uint8>(color), static_cast<EGE::Uint8>(std::max(0, static_cast<EGE::Int16>(color) + (static_cast<EGE::Int16>(particle.color) * 100 / 255) - 100)), 0}
            );
            counter++;
        }
        renderer.getTarget().draw(varr, renderer.getStates().sfStates());
    }
};

TESTCASE(basic)
{
    EGE::GUIGameLoop loop;

    auto scene = make<EGE::Scene>(&loop);
    auto& registry = scene->getRegistry();
    registry.addType<MyParticleSystem>();

    auto particleSystem = scene->addNewObject<MyParticleSystem>();

    auto camera = scene->addNewObject<EGE::Plain2DCamera>();
    camera->setScalingMode(EGE::ScalingMode::Fit);
    camera->setDisplaySize({200, 200});

    auto guiScreen = make<EGE::GUIScreen>(loop);
    auto sceneWidget = guiScreen->addNewWidget<EGE::SceneWidget>(scene);
    sceneWidget->setCamera(camera);
    loop.setCurrentGUIScreen(guiScreen);
    loop.openWindow(sf::VideoMode(500, 500), "EGE::PhysicalParticleSystem");

    return loop.run();
}

RUN_TESTS(physicalParticleSystem)
