#include <testsuite/Tests.h>
#include <ege/event.h>
#include <ege/gui.h>
#include <ege/scene.h>

TESTCASE(complexLoader)
{
    EGE::GUIGameLoop loop;
    loop.setResourceManager(make<EGE::GUIResourceManager>());

    // Load scene
    auto scene = make<EGE::Scene>(&loop);

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

    // Setup camera
    auto camera = scene->addNewObject<EGE::Plain2DCamera>();
    camera->setFollowObject(player);
    camera->setScalingMode(EGE::ScalingMode::Fit);
    camera->setDisplaySize({200, 200});

    // Setup keybinds
    auto keybinds = make<EGE::KeybindManager>();
    keybinds->addTrigger("place", sf::Mouse::Left, [scene, player]{ scene->addNewObject("CLBlock")->setPosition(player->getPosition()); });
    keybinds->addSwitch("test", sf::Keyboard::Space, [](bool b) { ege_log.info() << std::boolalpha << b << std::noboolalpha; });
    keybinds->addStrength("moveHorizontal", {sf::Keyboard::A, sf::Keyboard::D}, [player](float p) { player->setMotion({0.1*p, player->getMotion().y}); });
    keybinds->addStrength("moveVertical", {sf::Keyboard::W, sf::Keyboard::S}, [player](float p) { player->setMotion({player->getMotion().x, 0.1*p}); });
    EGE::KeybindManager::hook(keybinds, loop);

    // Setup GUI & display
    auto guiScreen = make<EGE::GUIScreen>(loop);
    auto sceneWidget = guiScreen->addNewWidget<EGE::SceneWidget>(scene);
    sceneWidget->setCamera(camera);
    loop.setCurrentGUIScreen(guiScreen);
    loop.openWindow(sf::VideoMode(500, 500), "Complex Test");
    return loop.run();
}

RUN_TESTS(complexLoader)
