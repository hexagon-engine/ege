#include <testsuite/Tests.h>
#include <ege/event.h>
#include <ege/gui.h>
#include <ege/scene.h>

TESTCASE(complexLoader)
{
    EGE::GUIGameLoop loop;
    loop.setResourceManager(make<EGE::GUIResourceManager>());

    // Load scene
    auto scene = make<EGE::Scene2D>(&loop);

    EGE::SceneObjectRegistry& sceneObjects = scene->getRegistry();
    if(!sceneObjects.loadFromFile("objects/complexloader.json"))
        return 1;

    if(!scene->loadFromFile("complexloader.json", "scenes/complexloader.json"))
        return 1;

    // Spawn player
    // TODO: Make better API for this!
    auto player = dynamic_cast<EGE::SceneObject2D*>(scene->getObjectByName("player"));
    if(!player)
        player = dynamic_cast<EGE::SceneObject2D*>(scene->addNewObject("CLPlayer").get());

    // Setup keybinds
    auto keybinds = make<EGE::KeybindManager>();
    keybinds->addTrigger("place", sf::Mouse::Left, [scene, player]{ EGE::SceneObject::cast<EGE::SceneObject2D>(scene->addNewObject("CLBlock"))->setPosition(player->getPosition()); });
    keybinds->addSwitch("L", sf::Keyboard::A, [player](bool p) { player->setMotion({-1*p, player->getMotion().y}); });
    keybinds->addSwitch("R", sf::Keyboard::D, [player](bool p) { player->setMotion({1*p, player->getMotion().y}); });
    keybinds->addSwitch("U", sf::Keyboard::W, [player](bool p) { player->setMotion({player->getMotion().x, -1*p}); });
    keybinds->addSwitch("D", sf::Keyboard::S, [player](bool p) { player->setMotion({player->getMotion().x, 1*p}); });
    loop.events<EGE::SystemEvent>().addHandler(keybinds);

    // Setup GUI & display
    auto guiScreen = make<EGE::GUIScreen>(loop);
    auto sceneWidget = guiScreen->addNewWidget<EGE::SceneWidget>(scene);
    loop.setCurrentGUIScreen(guiScreen);
    loop.openWindow(sf::VideoMode(500, 500), "Complex Test");
    return loop.run();
}

RUN_TESTS(complexLoader)
