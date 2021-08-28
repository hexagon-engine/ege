#include <ege/event.h>
#include <ege/gui.h>
#include <ege/scene.h>

int main()
{
    EGE::GUIGameLoop loop;
    loop.setResourceManager(make<EGE::GUIResourceManager>());
    loop.setMaxTicksPerSecond(60);

    // Load scene
    auto scene = make<EGE::Scene>(loop);

    EGE::SceneObjectRegistry& sceneObjects = scene->getRegistry();
    if(!sceneObjects.loadFromFile("objects/complexloader.json"))
        return 1;

    if(!scene->loadFromFile("complexloader.json", "scenes/complexloader.json"))
        return 1;

    // Spawn player
    // TODO: Make better API for this!
    auto player = scene->getObjectByName("player");
    if(!player)
    {
        ege_log.notice() << "No player found. Creating a new one.";
        player = scene->addNewObject("CLPlayer").get();
        player->setName("player");
    }

    // Open the window
    auto window = loop.openWindow(sf::VideoMode(500, 500), "Complex Test");

    // Setup keybinds
    auto keybinds = make<EGE::KeybindManager>();
    if(!keybinds->load())
    {
        // Use defaults
        keybinds->setKeybind("place", sf::Mouse::Left);
        keybinds->setKeybind("test", sf::Keyboard::Space);
        keybinds->setKeybind("moveHorizontal", {sf::Keyboard::A, sf::Keyboard::D});
        keybinds->setKeybind("moveVertical", {sf::Keyboard::W, sf::Keyboard::S});
    }

    auto& keybindHandler = EGE::KeybindManager::hook(keybinds, *player);
    keybindHandler.addTriggerHandler("place", [&scene, player]{ scene->addNewObject("CLBlock")->setPosition(player->getPosition()); });
    keybindHandler.addSwitchHandler("test", [](bool b) { ege_log.info() << std::boolalpha << b << std::noboolalpha; });
    keybindHandler.addStrengthHandler("moveHorizontal", [player](float p) { player->setMotion({1*p, player->getMotion().y}); });
    keybindHandler.addStrengthHandler("moveVertical", [player](float p) { player->setMotion({player->getMotion().x, 1*p}); });

    keybinds->save();

    // Setup GUI and camera
    auto guiScreen = window->setNewGUIScreen<EGE::GUIScreen>();
    auto sceneWidget = guiScreen->addNewWidget<EGE::SceneWidget>(scene);

    // Setup camera
    auto camera = scene->addNewObject<EGE::Plain2DCamera>();
    camera->setFollowObject(player);
    camera->setScalingMode(EGE::ScalingMode::Fit);
    camera->setDisplaySize({200, 200});
    sceneWidget->setCamera(camera);

    return loop.run();
}
