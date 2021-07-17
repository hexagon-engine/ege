#include <testsuite/Tests.h>
#include <ege/debug/Dump.h>
#include <ege/egeNetwork/EGEClient.h>
#include <ege/egeNetwork/EGEPacket.h>
#include <ege/egeNetwork/EGEServer.h>
#include <ege/egeNetwork/ServerNetworkController.h>
#include <ege/event/SystemEventHandler.h>
#include <ege/event/SystemWindow.h>
#include <ege/gui/GUIGameLoop.h>
#include <ege/gui/GUIScreen.h>
#include <ege/scene/Scene.h>
#include <ege/scene/SceneWidget.h>
#include <ege/scene/Plain2DCamera.h>
#include <ege/util/ObjectInt.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/ObjectString.h>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>

using EGE::EventResult;

TESTCASE(converter)
{
    EGE::SharedPtr<EGE::ObjectMap> map = make<EGE::ObjectMap>();
    map->addObject("packetTestInt", make<EGE::ObjectInt>(62452));
    map->addObject("String", make<EGE::ObjectString>("test"));
    map->addObject("IntAsString", make<EGE::ObjectString>("543"));
    EGE::SharedPtr<EGE::ObjectMap> subMap = make<EGE::ObjectMap>();
    subMap->addObject("MapTest", make<EGE::ObjectString>("YYYTEST555??"));
    map->addObject("SubMapTest", subMap);

    std::cerr << "Object: " << map->toString() << std::endl;

    EGE::EGEPacket packet(EGE::EGEPacket::Type::_Data, map);
    sf::Packet sfPacket = packet.serializeToSFML();

    std::cerr << "Hex dump: " << std::endl;
    EGE::hexDump(sfPacket.getData(), sfPacket.getDataSize(),
                EGE::HexDumpSettings{8});

    EGE::EGEPacket packet2;
    packet2.deserializeFromSFML(sfPacket);
    std::cerr << "Parsed object: " << packet2.getArgs()->toString() << std::endl;
    std::cerr << std::dec;

    return 0;
}

class MyObject : public EGE::SceneObject
{
public:
    EGE_SCENEOBJECT("MyObject")

    MyObject(EGE::Scene& owner)
    : EGE::SceneObject(owner) {}

    virtual void onInit() override
    {
        // only server side!
        if(!m_playerControlled && getOwner().isHeadless())
        {
            auto timer = make<EGE::Timer>(*this, EGE::Timer::Mode::Limited, EGE::Time(10.0, EGE::Time::Unit::Seconds), [this](std::string, EGE::Timer*) {
                if(getObjectId() == -1)
                    return;
                std::cerr << "[[[ object " << getObjectId() << " was removed ]]]" << std::endl;
                m_dead = true;
            });

            addTimer("timer", timer);
            setMotion(EGE::Vec2d(rand() % 5 - 2, rand() % 5 - 2));
        }
        setRotation(rand() % 360);
        m_scale = EGE::Vec2d(rand() % 2 + 1, rand() % 2 + 1);
    }

    virtual void render(EGE::Renderer& renderer) const override
    {
        EGE::SceneObject::render(renderer);

        sf::RectangleShape rs(sf::Vector2f(10.f, 10.f));
        rs.setPosition(getPosition().x - 5, getPosition().y - 5);
        rs.setRotation(getRotation());
        rs.setScale(m_scale.x, m_scale.y);
        rs.setOrigin(m_origin.x, m_origin.y);
        renderer.getTarget().draw(rs);
    }

    virtual void move(bool moving, int dir)
    {
        // TODO: Use KeybindManager
        if(moving)
        {
            switch(dir)
            {
                case 0: setMotion({-5.0, getMotion().y}); break; // a
                case 1: setMotion({getMotion().y, 5.0}); break; // s
                case 2: setMotion({5.0, getMotion().y}); break; // d
                case 3: setMotion({getMotion().y, -5.0}); break; // w
            }
        }
        else
        {
            switch(dir)
            {
                case 0: if(getMotion().x == -5.f) setMotion({0.0, getMotion().y}); break; // a
                case 1: if(getMotion().y == 5.f) setMotion({getMotion().y, 0.0}); break; // s
                case 2: if(getMotion().x == 5.f) setMotion({0.0, getMotion().y}); break; // d
                case 3: if(getMotion().y == -5.f) setMotion({getMotion().y, 0.0}); break; // w
            }
        }
        setMainChanged();
    }
    bool m_playerControlled;
    EGE::Vec2d m_scale;
    EGE::Vec2d m_origin;
};

class MyObjectServerController : public EGE::ServerNetworkController
{
public:
    MyObjectServerController(EGE::SceneObject& object, EGE::EGEServer& server)
    : EGE::ServerNetworkController(object, server) {}

    virtual void handleRequest(const EGE::ControlPacket& data) override
    {
        std::string type = data.getType();
        auto& object = (MyObject&)getObject();
        bool moving = data.getArgs()->getObject("moving").asBoolean().valueOr(false);
        int dir = data.getArgs()->getObject("dir").asInt().valueOr(0);

        if(type == "move")
            object.move(moving, dir);
    }
};

class MyObjectClientController : public EGE::ClientNetworkController
{
public:
    MyObjectClientController(EGE::SceneObject& object, EGE::EGEClient& client)
    : EGE::ClientNetworkController(object, client) {}

    virtual void handleRequest(const EGE::ControlPacket& data) override
    {
        std::string type = data.getType();
        auto& object = (MyObject&)getObject();
        bool moving = data.getArgs()->getObject("moving").asBoolean().valueOr(false);
        int dir = data.getArgs()->getObject("dir").asInt().valueOr(0);

        if(type == "move")
            object.move(moving, dir);
    }
};

class MyServer : public EGE::EGEServer
{
public:
    MyServer()
    : EGE::EGEServer(rand() % 63536 + 2000)
    {
        setVersion(1);
        setVersionString("EGE Test");
    }

    virtual EGE::SharedPtr<EGE::ServerNetworkController> makeController(EGE::SceneObject& object) override
    {
        if(object.getType()->getId() == "MyObject")
        {
            // MyObject is controlled by MyObjectServerController
            return make<MyObjectServerController>(object, *this);
        }

        // object cannot be controlled
        return nullptr;
    }

    virtual EGE::EventResult onLogin(EGE::EGEClientConnection& client, EGE::SharedPtr<EGE::ObjectMap> data) override
    {
        // Synchronize client with server.
        EGE::EGEServer::onLogin(client, data);

        // Spawn SceneObject that will be controlled by this client.
        ASSERT(getScene());
        auto sceneObject = getScene()->addNewObject<MyObject>();
        std::cerr << "Adding Object to Scene" << std::endl;

        // Set SceneObject to be controlled by this Client.
        setDefaultController(client, sceneObject.get());
        addAdditionalController(client, *getScene()->getObject(-1));

        return EGE::EventResult::Success;
    }
};

class MyClient : public EGE::EGEClient
{
public:
    MyClient(unsigned short port)
    : EGE::EGEClient(sf::IpAddress::LocalHost, port) { setVersion(1); setVersionString("EGE Test"); }

    virtual EGE::SharedPtr<EGE::ClientNetworkController> makeController(EGE::SceneObject& object) override
    {
        if(object.getType()->getId() == "MyObject")
        {
            // MyObject is controlled by MyObjectClientController
            return make<MyObjectClientController>(object, *this);
        }

        // object cannot be controlled
        return nullptr;
    }

    void move(bool moving, int dir)
    {
        EGE::SharedPtr<EGE::ObjectMap> _map = make<EGE::ObjectMap>();
        _map->addInt("moving", moving);
        _map->addInt("dir", dir);
        requestControl(nullptr, EGE::ControlPacket("move", _map));
    }

    void move2(bool moving, int dir)
    {
        EGE::SharedPtr<EGE::ObjectMap> _map = make<EGE::ObjectMap>();
        _map->addInt("moving", moving);
        _map->addInt("dir", dir);
        requestControl(getScene()->getObject(-1).get(), EGE::ControlPacket("move", _map));
    }
};

TESTCASE(server)
{
    srand(time(nullptr));

    MyServer server;

    server.setMinimalTickTime(EGE::Time(1 / 60.0, EGE::Time::Unit::Seconds)); //60 tps

    auto scene = make<EGE::Scene>(nullptr);

    auto timer = make<EGE::Timer>(server, EGE::Timer::Mode::Infinite, EGE::Time(2.0, EGE::Time::Unit::Seconds), [scene](std::string, EGE::Timer*) {
        auto object = scene->addNewObject<MyObject>();
        object->setPosition({(double)(rand() % 50 - 25), (double)(rand() % 50 - 25)});
    });
    server.addTimer("timer", timer);

    server.setScene(scene);

    // Register SceneObject types for Client.
    scene->getRegistry().addType<MyObject>();

    return server.run();
}

class MySystemEventHandler : public EGE::DefaultSystemEventHandler
{
public:
    MySystemEventHandler(EGE::SFMLSystemWindow& window, EGE::SharedPtr<MyClient> client)
    : EGE::DefaultSystemEventHandler(window), m_client(client) {}

    void onKeyPress(sf::Event::KeyEvent& event)
    {
        switch(event.code)
        {
        case sf::Keyboard::A: m_client->move(true, 0);
            break;
        case sf::Keyboard::S: m_client->move(true, 1);
            break;
        case sf::Keyboard::D: m_client->move(true, 2);
            break;
        case sf::Keyboard::W: m_client->move(true, 3);
            break;
        case sf::Keyboard::Left: m_client->move2(true, 0);
            break;
        case sf::Keyboard::Down: m_client->move2(true, 1);
            break;
        case sf::Keyboard::Right: m_client->move2(true, 2);
            break;
        case sf::Keyboard::Up: m_client->move2(true, 3);
            break;
        default:
            break;
        }
    }

    void onKeyRelease(sf::Event::KeyEvent& event)
    {
        switch(event.code)
        {
        case sf::Keyboard::A: m_client->move(false, 0);
            break;
        case sf::Keyboard::S: m_client->move(false, 1);
            break;
        case sf::Keyboard::D: m_client->move(false, 2);
            break;
        case sf::Keyboard::W: m_client->move(false, 3);
            break;
        case sf::Keyboard::Left: m_client->move2(false, 0);
            break;
        case sf::Keyboard::Down: m_client->move2(false, 1);
            break;
        case sf::Keyboard::Right: m_client->move2(false, 2);
            break;
        case sf::Keyboard::Up: m_client->move2(false, 3);
            break;
        default:
            break;
        }
    }

private:
    EGE::SharedPtr<MyClient> m_client;
};

class MyGameLoop : public EGE::GUIGameLoop
{
    EGE::SharedPtr<MyClient> m_client;
    EGE::SharedPtr<EGE::Plain2DCamera> m_camera;
    int m_port;

public:
    MyGameLoop(int port)
    : m_port(port) {}

    virtual EventResult load() override
    {
        // Create GUI and assign it to client.
        auto gui = make<EGE::GUIScreen>(*this);
        setCurrentGUIScreen(gui);

        // Create client - define server IP and port.
        m_client = make<MyClient>(m_port);

        // Create scene and assign it to client.
        auto scene = make<EGE::Scene>(this);
        m_client->setScene(scene);

        // Create SceneWidget to be displayed in the window.
        gui->addWidget(make<EGE::SceneWidget>(*gui, scene));

        // Register SceneObject types for Client.
        scene->getRegistry().addType<MyObject>();

        // Set exit handler for Client. It will be called when client is
        // disconnected.
        m_client->events<EGE::ExitEvent>().add([this](EGE::ExitEvent &event) {
            std::cerr << "CLIENT CLOSED, R=" << event.returnValue << std::endl;
            exit(event.returnValue);
            return EGE::EventResult::Success;
        });

        // Add keybind handler. It will pass keyboard events to Controller.
        events<EGE::SystemEvent>().addHandler<MySystemEventHandler>(getWindow(), m_client);

        // Initialize Camera.
        m_camera = scene->addNewObject<EGE::Plain2DCamera>(nullptr);
        m_camera->setScalingMode(EGE::ScalingMode::Centered);
        scene->setCamera(m_camera);

        // Set our Client as sub-loop. It will automatically connect to server
        // now.
        if(!addSubLoop(m_client))
            return EGE::EventResult::Failure;

        return EGE::EventResult::Success;
    }

    virtual void onTick(long long tick) override
    {
        EGE::GUIGameLoop::onTick(tick);
        // Camera: follow currently controlled object.
        auto controller = m_client->getDefaultController();
        if(controller)
            m_camera->setParent(dynamic_cast<EGE::SceneObject*>(&controller->getObject()));
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
    loop.openWindow(sf::VideoMode(300, 300), "EGE Protocol Test");
    loop.getWindow().setKeyRepeatEnabled(false);
    loop.setMaxTicksPerSecond(60);

    // Run main loop.
    return loop.run();
}

RUN_TESTS(egeNetwork)
