#include <testsuite/Tests.h>
#include <ege/gfx.h>
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
    : EGE::PhysicalParticleSystem<MyParticle>(scene) {}

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
            varr[2*counter] = sf::Vertex({static_cast<float>(particle.position.x-particle.motion.x), static_cast<float>(particle.position.y-particle.motion.y)},
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

class SliderAndValue : public EGE::Widget
{
public:
    SliderAndValue(EGE::Widget& parent, EGE::String id = "SliderAndValue")
    : EGE::Widget(parent, id)
    {
        setPadding({"2px", "2px"});
        setSize({"1N", "40px"});
        layoutDirection = EGE::LayoutElement::Direction::Vertical;

        m_nameLabel = addNewWidget<EGE::Label>();
        m_nameLabel->setColor(sf::Color::White);

        auto sliderContainer = addNewWidget<EGE::Widget>();
        {
            sliderContainer->setSize({"1N", "20px"});
            m_slider = sliderContainer->addNewWidget<EGE::Slider>();

            // TODO: Add margins to make this not needed
            auto labelContainer = sliderContainer->addNewWidget<EGE::Widget>();
            labelContainer->setPadding({"4px", "0px"});
            labelContainer->setSize({"90px", "1N"});
            {
                m_valueLabel = labelContainer->addNewWidget<EGE::Label>();
                m_valueLabel->setColor(sf::Color::White);
            }
        }
        m_slider->events<EGE::SliderSlideEvent>().add([&](EGE::SliderSlideEvent& event) { m_valueLabel->setString(std::to_string(event.value)); return EGE::EventResult::Success; });
    }

    EGE::Slider& slider() { return *m_slider; }

    void setName(const EGE::String& name) { m_nameLabel->setString(name); }

private:
    EGE::SharedPtr<EGE::Slider> m_slider;
    EGE::SharedPtr<EGE::Label> m_nameLabel;
    EGE::SharedPtr<EGE::Label> m_valueLabel;
};

class ParticleLabel : public EGE::Label
{
public:
    ParticleLabel(EGE::Widget& parent, MyParticleSystem& particleSystem)
    : EGE::Label(parent), m_particleSystem(particleSystem) {}

    virtual void onTick() override
    {
        EGE::String info;
        info += "Particle count: " + std::to_string(m_particleSystem.getParticleCount());
        info += " FPS: " + std::to_string(getLoop().getLastTPS());
        setString(info);
    }

private:
    MyParticleSystem& m_particleSystem;
};

// We need separate GUI screen because we don't have resourcemanager before loop.run()
class MyGuiScreen : public EGE::GUIScreen
{
public:
    MyGuiScreen(EGE::Window& window)
    : EGE::GUIScreen(window) {}

    virtual void onCreate() override
    {
        // TODO: Make this work
        //m_sceneWidget->setNewFilter<EGE::ShaderFilterWithRenderTexture>(getLoop().getResourceManager()->getShader("filter"));
    }

    void setSceneWidget(EGE::SharedPtr<EGE::SceneWidget> sceneWidget) { m_sceneWidget = sceneWidget; }

private:
    EGE::SharedPtr<EGE::SceneWidget> m_sceneWidget;
};

class GameLoop : public EGE::GUIGameLoop
{
public:
    bool dumpProfiler = false;

protected:
    virtual void onProfilerResults(EGE::Profiler const& profiler) override
    {
        if(dumpProfiler)
        {
            ege_log.info() << "Profiler dump: " << profiler.toString();
            dumpProfiler = false;
        }
    }
};


TESTCASE(basic)
{
    EGE::GlobalConfig::set(EGE::GlobalConfig::Option::Profiler_Enable);
    GameLoop loop;
    loop.setMaxTicksPerSecond(60);

    auto resourceManager = make<EGE::GUIResourceManager>();
    resourceManager->registerDefaultFont("font.ttf");
    resourceManager->registerShader("filter", {"filter.vert", "filter.frag"});
    loop.setResourceManager(resourceManager);

    auto scene = make<EGE::Scene>(loop);
    auto& registry = scene->getRegistry();
    registry.addType<MyParticleSystem>();

    auto particleSystem = scene->addNewObject<MyParticleSystem>();

    auto camera = scene->addNewObject<EGE::Plain2DCamera>();
    camera->setScalingMode(EGE::ScalingMode::Fit);
    camera->setDisplaySize({200, 200});

    auto window = loop.openWindow(sf::VideoMode(500, 500), "EGE::PhysicalParticleSystem");
    auto guiScreen = window->setNewGUIScreen<MyGuiScreen>();
    guiScreen->setPadding({"5px", "5px"});

    {
        auto infoLabel = guiScreen->addNewWidget<ParticleLabel>(*particleSystem);

        // HACK: layout align should work
        infoLabel->setPosition({"0px", "0px"});
        infoLabel->setSize({"100%", "20px"});
        infoLabel->setTextAlign(EGE::Label::Align::Right);
        infoLabel->setColor(sf::Color::White);

        auto controlsWidget = guiScreen->addNewWidget<EGE::GroupBox>();
        controlsWidget->setLabel("Controls");
        controlsWidget->setLabelColor(EGE::Colors::white);
        controlsWidget->setSize({"300px", "1N"});
        controlsWidget->layoutDirection = EGE::LayoutElement::Direction::Vertical;
        {
            auto killAllParticles = controlsWidget->addNewWidget<EGE::Button>();
            killAllParticles->events<EGE::Button::ClickEvent>().add([&](EGE::Button::ClickEvent&) {
                particleSystem->killAllParticles();
                return EGE::EventResult::Success;
            });
            killAllParticles->setLabel("Kill all particles");
            killAllParticles->setSize({"1N", "25px"});

            auto dumpProfiler = controlsWidget->addNewWidget<EGE::Button>();
            dumpProfiler->events<EGE::Button::ClickEvent>().add([&](EGE::Button::ClickEvent&) {
                loop.dumpProfiler = true;
                return EGE::EventResult::Success;
            });
            dumpProfiler->setLabel("Dump profiler");
            dumpProfiler->setSize({"1N", "25px"});

            auto showOverlayCheckbox = controlsWidget->addNewWidget<EGE::CheckBox>();
            showOverlayCheckbox->events<EGE::Button::ClickEvent>().add([showOverlayCheckbox](EGE::Button::ClickEvent&) {
                if(showOverlayCheckbox->isChecked())
                    EGE::GlobalConfig::set(EGE::GlobalConfig::Option::SceneObject_Overlay);
                else
                    EGE::GlobalConfig::unset(EGE::GlobalConfig::Option::SceneObject_Overlay);
                return EGE::EventResult::Success;
            });
            showOverlayCheckbox->setLabel("SceneObject Overlay");
            showOverlayCheckbox->setLabelColor(EGE::Colors::white);
            showOverlayCheckbox->setSize({"1N", "25px"});

            auto gravitySlider = controlsWidget->addNewWidget<SliderAndValue>();
            gravitySlider->setName("Gravity");
            gravitySlider->slider().events<EGE::SliderSlideEvent>().add([&](auto& event) {
                particleSystem->setGravity({0, event.value, 0});
                return EGE::EventResult::Success;
            });
            gravitySlider->slider().setMaxValue(0.05);
            gravitySlider->slider().setMinValue(0);
            gravitySlider->slider().setValue(0.002);

            auto startMotionSlider = controlsWidget->addNewWidget<SliderAndValue>();
            startMotionSlider->setName("Start motion");
            startMotionSlider->slider().events<EGE::SliderSlideEvent>().add([&](auto& event) {
                particleSystem->setStartMotion({0, -event.value, 0});
                return EGE::EventResult::Success;
            });
            startMotionSlider->slider().setMaxValue(10);
            startMotionSlider->slider().setMinValue(0);
            startMotionSlider->slider().setValue(1.5);

            auto angleSlider = controlsWidget->addNewWidget<SliderAndValue>();
            angleSlider->setName("Start motion angle random");
            angleSlider->slider().events<EGE::SliderSlideEvent>().add([&](auto& event) {
                particleSystem->setStartMotionAngleRandom(event.value);
                return EGE::EventResult::Success;
            });
            angleSlider->slider().setMaxValue(360);
            angleSlider->slider().setMinValue(0);
            angleSlider->slider().setStep(1);
            angleSlider->slider().setValue(25);

            auto valueRandomSlider = controlsWidget->addNewWidget<SliderAndValue>();
            valueRandomSlider->setName("Start motion value random");
            valueRandomSlider->slider().events<EGE::SliderSlideEvent>().add([&](auto& event) {
                particleSystem->setStartMotionValueRandom(event.value);
                return EGE::EventResult::Success;
            });
            valueRandomSlider->slider().setMaxValue(0.9);
            valueRandomSlider->slider().setMinValue(0.0);
            valueRandomSlider->slider().setValue(0.3);

            auto lifetimeSlider = controlsWidget->addNewWidget<SliderAndValue>();
            lifetimeSlider->setName("Life time");
            lifetimeSlider->slider().events<EGE::SliderSlideEvent>().add([&](auto& event) {
                particleSystem->setParticleLifeTime(event.value);
                return EGE::EventResult::Success;
            });
            lifetimeSlider->slider().setMaxValue(1200);
            lifetimeSlider->slider().setMinValue(1);
            lifetimeSlider->slider().setStep(1);
            lifetimeSlider->slider().setValue(120);

            auto spawnChanceSlider = controlsWidget->addNewWidget<SliderAndValue>();
            spawnChanceSlider->setName("Spawn chance");
            spawnChanceSlider->slider().events<EGE::SliderSlideEvent>().add([&](auto& event) {
                particleSystem->setSpawnChance(event.value);
                return EGE::EventResult::Success;
            });
            spawnChanceSlider->slider().setMaxValue(50);
            spawnChanceSlider->slider().setMinValue(0.01);
            spawnChanceSlider->slider().setValue(1);
            spawnChanceSlider->slider().setValueTransform([](auto v) { return std::exp2(v)-1; }, [](auto v) { return std::log2(v+1); });
        
            auto spawnRangeXSlider = controlsWidget->addNewWidget<SliderAndValue>();
            spawnRangeXSlider->setName("Spawn range X");
            spawnRangeXSlider->slider().events<EGE::SliderSlideEvent>().add([&](auto& event) {
                auto rect = particleSystem->getSpawnRect();
                rect.size.x = event.value;
                rect.center();
                particleSystem->setSpawnRect(rect);
                return EGE::EventResult::Success;
            });
            spawnRangeXSlider->slider().setMaxValue(200);
            spawnRangeXSlider->slider().setMinValue(1);
            spawnRangeXSlider->slider().setStep(1);
            spawnRangeXSlider->slider().setValue(4);
        
            auto spawnRangeYSlider = controlsWidget->addNewWidget<SliderAndValue>();
            spawnRangeYSlider->setName("Spawn range Y");
            spawnRangeYSlider->slider().events<EGE::SliderSlideEvent>().add([&](auto& event) {
                auto rect = particleSystem->getSpawnRect();
                rect.size.y = event.value;
                rect.center();
                particleSystem->setSpawnRect(rect);
                return EGE::EventResult::Success;
            });
            spawnRangeYSlider->slider().setMaxValue(200);
            spawnRangeYSlider->slider().setMinValue(1);
            spawnRangeYSlider->slider().setStep(1);
            spawnRangeYSlider->slider().setValue(10);
        
            auto cameraZoomSlider = controlsWidget->addNewWidget<SliderAndValue>();
            cameraZoomSlider->setName("Zoom");
            cameraZoomSlider->slider().events<EGE::SliderSlideEvent>().add([&](auto& event) {
                camera->setZoom(event.value);
                return EGE::EventResult::Success;
            });
            cameraZoomSlider->slider().setMaxValue(10);
            cameraZoomSlider->slider().setMinValue(0.1);
            cameraZoomSlider->slider().setValue(1);
        }

        auto sceneWidget = guiScreen->addNewWidget<EGE::SceneWidget>(scene);
        sceneWidget->setCamera(camera);
        guiScreen->setSceneWidget(sceneWidget);
    }

    return loop.run();
}

RUN_TESTS(physicalParticleSystem)
