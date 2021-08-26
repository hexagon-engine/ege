#include <ege/core.h>
#include <ege/gui.h>
#include <ege/scene.h>
#include <ege/util.h>

using namespace EGE;

class PhysicalSO : public SceneObject
{
public:
    EGE_SCENEOBJECT("PhysicalSO")

    PhysicalSO(Scene& owner)
    : SceneObject(owner) {}

    virtual void onInit() override
    {
        addNewBehaviour<PhysicsBehaviour>();
        setPosition({EGE::randomDouble(-50, 50), EGE::randomDouble(-50, 50), 0});
        setMotion({EGE::randomDouble(-1, 1), EGE::randomDouble(-3, 3), 0});
    }
};

int main()
{
    GlobalConfig::set(GlobalConfig::Option::Profiler_Enable);

    GUIGameLoop loop;
    loop.setMaxTicksPerSecond(60);
    auto window = loop.openWindow(sf::VideoMode(600, 600), "EGE::PhysicsBehaviour");

    auto resourceManager = make<GUIResourceManager>();
    resourceManager->registerDefaultFont("font.ttf");
    loop.setResourceManager(resourceManager);

    auto scene = make<Scene>(loop);

    auto& physics = scene->addNewBehaviour<GlobalPhysicsBehaviour>();
    physics.setGravity({0, 0.1, 0});

    auto& registry = scene->getRegistry();
    registry.addType<PhysicalSO>();
    if(!registry.loadFromFile("objects.json"))
        return 1;

    scene->addNewObject<PhysicalSO>();
    scene->addNewObject<PhysicalSO>();
    scene->addNewObject<PhysicalSO>();

    auto gui = window->setNewGUIScreen<GUIScreen>();
    auto sceneWidget = gui->addNewWidget<SceneWidget>(scene);

    auto label = gui->addNewWidget<Label>();
    label->setString("Loading...");
    label->setColor(Colors::red);
    label->setPosition({"0px", "0px"});
    label->setSize({"100%", "30px"});

    loop.events<TickEvent>().add([&](auto&) {
        auto tps = loop.getLastTicksPerSecond();
        label->setString(std::to_string(tps));
        return EventResult::Success;
    });

    auto camera = scene->addNewObject<Plain2DCamera>();
    camera->setDisplaySize({600, 600});
    camera->setScalingMode(ScalingMode::Fit);
    sceneWidget->setCamera(camera);

    return loop.run();
}
