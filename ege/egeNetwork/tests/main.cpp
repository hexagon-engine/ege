#include <testsuite/Tests.h>
#include <ege/debug/Dump.h>
#include <ege/egeNetwork/EGEClient.h>
#include <ege/egeNetwork/EGEClientConnection.h>
#include <ege/egeNetwork/EGEPacket.h>
#include <ege/egeNetwork/EGEServer.h>
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
        if(getOwner().isHeadless())
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
                case 1: setMotion({getMotion().x, 5.0}); break; // s
                case 2: setMotion({5.0, getMotion().y}); break; // d
                case 3: setMotion({getMotion().x, -5.0}); break; // w
            }
        }
        else
        {
            switch(dir)
            {
                case 0: if(getMotion().x == -5.f) setMotion({0.0, getMotion().y}); break; // a
                case 1: if(getMotion().y == 5.f) setMotion({getMotion().x, 0.0}); break; // s
                case 2: if(getMotion().x == 5.f) setMotion({0.0, getMotion().y}); break; // d
                case 3: if(getMotion().y == -5.f) setMotion({getMotion().x, 0.0}); break; // w
            }
        }
        setMainChanged();
    }
    EGE::Vec2d m_scale;
    EGE::Vec2d m_origin;
};

class MyServer : public EGE::EGEServer
{
public:
    MyServer()
    {
        setVersion(1); setVersionString("EGE Test");
        registerController("move", [](EGE::SceneObject& object, EGE::SharedPtr<EGE::ObjectMap> args) {
            // FIXME: This dynamic_cast should not be needed
            MyObject* myObject = dynamic_cast<MyObject*>(&object);
            if(myObject)
            {
                myObject->move(args->get("moving").asBoolean().valueOr(false), args->get("dir").asInt().valueOr(0));
            }
        });
    }

    virtual void onLogin(EGE::EGEClientConnection& client) override
    {
        // Spawn SceneObject that will be controlled by this client.
        ASSERT(getScene());
        auto sceneObject = getScene()->addNewObject<MyObject>();
        std::cerr << "Adding Object to Scene" << std::endl;

        client.setControlledSceneObject(sceneObject.get());
    }
};

class MyClient : public EGE::EGEClient
{
public:
    MyClient() { setVersion(1); setVersionString("EGE Test"); }

    void move(bool moving, int dir)
    {
        auto object = make<EGE::ObjectMap>();
        object->addBoolean("moving", moving);
        object->addInt("dir", dir);
        control("move", object);
    }

    void setCamera(EGE::SharedPtr<EGE::Plain2DCamera> camera) { m_camera = camera; }

    virtual void onSetDefaultController(EGE::SceneObject const* object) override
    {
        m_camera->setFollowObject(object);
    }

    virtual EGE::EventResult onFinish(int exitCode) override
    {
        getParentLoop()->exit(exitCode);
        return EGE::EventResult::Success;
    }

private:
    EGE::SharedPtr<EGE::Plain2DCamera> m_camera;
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

    server.listen(rand() % 1024 + 64512);
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
        m_client = make<MyClient>();

        // Create scene and assign it to client.
        auto scene = make<EGE::Scene>(this);
        m_client->setScene(scene);

        // Register SceneObject types for Client.
        scene->getRegistry().addType<MyObject>();

        // Add keybind handler. It will pass keyboard events to Controller.
        events<EGE::SystemEvent>().addHandler<MySystemEventHandler>(getWindow(), m_client);

        // Initialize Camera.
        m_camera = scene->addNewObject<EGE::Plain2DCamera>(nullptr);
        m_camera->setScalingMode(EGE::ScalingMode::Centered);
        m_client->setCamera(m_camera);

        // Create SceneWidget to be displayed in the window, and assign Camera to it.
        auto sceneWidget = gui->addNewWidget<EGE::SceneWidget>(*gui, scene);
        sceneWidget->setCamera(m_camera);

        // Add as subloop to handle events.
        addSubLoop(m_client);

        // Connect to server.
        // TODO: This should automatically add as subloop if required!
        if(!m_client->connect(sf::IpAddress::LocalHost, m_port))
            return EGE::EventResult::Failure;

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
    EGE::GlobalConfig::enableAllDebug();
    MyGameLoop loop(PORT);
    loop.openWindow(sf::VideoMode(300, 300), "EGE Protocol Test");
    loop.getWindow().setKeyRepeatEnabled(false);
    loop.setMaxTicksPerSecond(60);

    // Run main loop.
    return loop.run();
}

RUN_TESTS(egeNetwork)
