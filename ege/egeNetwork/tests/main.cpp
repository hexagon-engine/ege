#include <testsuite/Tests.h>
#include <ege/debug/Dump.h>
#include <ege/egeNetwork/EGEClient.h>
#include <ege/egeNetwork/EGEPacket.h>
#include <ege/egeNetwork/EGEServer.h>
#include <ege/event/SystemWindow.h>
#include <ege/gui/GUIGameLoop.h>
#include <ege/gui/GUIScreen.h>
#include <ege/scene/Scene2D.h>
#include <ege/scene/SceneWidget.h>
#include <ege/scene/TexturedObject2D.h>
#include <ege/util/ObjectInt.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/ObjectString.h>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>

TESTCASE(converter)
{
    std::shared_ptr<EGE::ObjectMap> map = std::make_shared<EGE::ObjectMap>();
    map->addObject("packetTestInt", std::make_shared<EGE::ObjectInt>(62452));
    map->addObject("String", std::make_shared<EGE::ObjectString>("test"));
    map->addObject("IntAsString", std::make_shared<EGE::ObjectString>("543"));
    std::shared_ptr<EGE::ObjectMap> subMap = std::make_shared<EGE::ObjectMap>();
    subMap->addObject("MapTest", std::make_shared<EGE::ObjectString>("YYYTEST555??"));
    map->addObject("SubMapTest", subMap);

    std::cerr << "Object: " << map->toString() << std::endl;

    EGE::EGEPacket packet(EGE::EGEPacket::Type::_Data, map);
    sf::Packet sfPacket = packet.toSFMLPacket();

    std::cerr << "Hex dump: " << std::endl;
    EGE::hexDump(sfPacket.getData(), sfPacket.getDataSize(), EGE::HexDumpSettings{8});

    EGE::EGEPacket packet2(sfPacket);
    std::cerr << "Parsed object: " << packet2.getArgs()->toString() << std::endl;
    std::cerr << std::dec;
}

class MyObject : public EGE::SceneObject2D
{
public:
    MyObject(EGE::Scene* owner)
    : EGE::SceneObject2D(owner, "test-egeNetwork:MyObject")
    {
        auto timer = std::make_shared<EGE::Timer>(this, EGE::Timer::Mode::Limited, EGE::Time(10.0, EGE::Time::Unit::Seconds));
        timer->setCallback([this](std::string, EGE::Timer*) {
                                std::cerr << "[[[ object " << getObjectId() << " was removed ]]]" << std::endl;
                                m_dead = true;
                           });

        addTimer("timer", timer);
    }

    virtual void render(sf::RenderTarget& target) const
    {
        EGE::SceneObject2D::render(target);
        sf::RectangleShape rs(sf::Vector2f(10.f, 10.f));
        rs.setPosition(getPosition() - sf::Vector2f(5.f, 5.f));
        target.draw(rs);
    }
};

TESTCASE(server)
{
    srand(time(nullptr));
    int PORT = rand() % 63536 + 2000;

    auto serverThread = [PORT]() {
        EGE::EGEServer server(PORT);

        // Set initialize handler for Server. It will be called before starting listening.
        server.setInitializeHandler([](EGE::EGEGame::GPOM* gpom) {
                                gpom->sceneObjectCreators.add("test-egeNetwork:MyObject", new std::function(
                                                                [](EGE::Scene* scene)->std::shared_ptr<EGE::SceneObject> {
                                                                    return std::make_shared<MyObject>(scene);
                                                                }));
                                return true;
                            });

        auto scene = std::make_shared<EGE::Scene2D>(nullptr);

        auto timer = std::make_shared<EGE::Timer>(&server, EGE::Timer::Mode::Infinite, EGE::Time(2.0, EGE::Time::Unit::Seconds));
        timer->setCallback([scene](std::string, EGE::Timer*) {
                                auto object = std::make_shared<MyObject>(scene.get());
                                object->setPosition(sf::Vector2f(rand() % 50 - 25, rand() % 50 - 25));
                                scene->addObject(object);
                           });

        server.addTimer("timer", timer);

        server.setScene(scene);
        return server.run();
    };
    sf::Thread thread1(serverThread);
    thread1.launch();
}

class MyGameLoop : public EGE::GUIGameLoop
{
    std::shared_ptr<EGE::EGEClient> m_client;
    int m_port;

public:
    MyGameLoop(int port)
    : m_port(port) {}

    EGE::EventResult onLoad()
    {
        // Call base onLoad to create profiler, load resources etc.
        if(EGE::GUIGameLoop::onLoad() == EGE::EventResult::Failure)
            return EGE::EventResult::Failure;

        // Create GUI and assign it to client.
        auto gui = std::make_shared<EGE::GUIScreen>(this);
        setCurrentGUIScreen(gui);

        // Create client - define server IP and port.
        m_client = std::make_shared<EGE::EGEClient>(sf::IpAddress::LocalHost, m_port);

        // Create scene and assign it to client.
        auto scene = std::make_shared<EGE::Scene2D>(this);
        m_client->setScene(scene);

        // Create SceneWidget to be displayed in the window.
        gui->addWidget(std::make_shared<EGE::SceneWidget>(gui.get(), scene));

        // Set initialize handler for Client. It will be called before connecting.
        m_client->setInitializeHandler([](EGE::EGEGame::GPOM* gpom) {
                                gpom->sceneObjectCreators.add("test-egeNetwork:MyObject", new std::function(
                                                                [](EGE::Scene* scene)->std::shared_ptr<EGE::SceneObject> {
                                                                    return std::make_shared<MyObject>(scene);
                                                                }));
                                return true;
                            });

        // Set exit handler for Client. It will be called when client is disconnected.
        m_client->setExitHandler([this](int retVal) {
                                    std::cerr << "CLIENT CLOSED, R=" << retVal << std::endl;
                                    exit(retVal);
                                 });

        // Set our Client as sub-loop. It will automatically connect to server now.
        setSubLoop(m_client);

        return EGE::EventResult::Success;
    }
};

TESTCASE(client)
{
    int PORT = 0;

    // Ask user for port.
    // Yes, there is no error handling...
    std::cout << "Enter port: ";
    std::cin >> PORT;

    // Create GameLoop and window.
    MyGameLoop loop(PORT);
    loop.setWindow(std::make_shared<EGE::SFMLSystemWindow>(sf::VideoMode(300, 300), "EGE Protocol Test"));

    // Run main loop.
    loop.run();

    // Here doing ANYTHING is NOT SAFE!!! (because of destructors)
}

RUN_TESTS(egeNetwork)
