/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "EGEServer.h"

#include "EGEClientConnection.h"
#include "EGEPacket.h"

#include <ege/asyncLoop/AsyncTask.h>
#include <ege/controller/ControlObject.h>
#include <ege/debug/Dump.h>
#include <ege/network/ClientConnection.h>
#include <iomanip>
#include <iostream>

namespace EGE
{

EventResult EGEServer::onClientConnect(ClientConnection* client)
{
    sf::Lock lock(m_clientsAccessMutex);
    // FIXME: it should be asynchronous

    if(!client->send(EGEPacket::generate_Ping()))
        return EventResult::Failure;
    if(!client->send(EGEPacket::generate_ProtocolVersion(EGE_PROTOCOL_VERSION)))
        return EventResult::Failure;
    if(!client->send(EGEPacket::generateSLoginRequest(getLoginData(client))))
        return EventResult::Failure;

    return EventResult::Success;
}

EventResult EGEServer::onClientDisconnect(ClientConnection* client)
{
    sf::Lock lock(m_clientsAccessMutex);
    if(!client->send(EGEPacket::generateSDisconnectReason("Disconnected")))
        return EventResult::Failure;

    return EventResult::Success;
}

void EGEServer::setScene(std::shared_ptr<Scene> scene)
{
    if(!scene)
    {
        m_controllersForObjects.clear();
        EGEGame::setScene(scene);
        return;
    }

    scene->setAddObjectCallback([this](std::shared_ptr<SceneObject> object) {
                                    // Add controller to controller map.
                                    m_controllersForObjects[object->getObjectId()] = makeController(object);
                                    sendToAll(EGEPacket::generateSSceneObjectCreation(object, object->getId()));
                                });
    scene->setRemoveObjectCallback([this](std::shared_ptr<SceneObject> object) {
                                    // Remove controller from controller map.
                                    auto it = m_controllersForObjects.find(object->getObjectId());
                                    if(it != m_controllersForObjects.end())
                                        m_controllersForObjects.erase(it);

                                    sendToAll(EGEPacket::generateSSceneObjectDeletion(object->getObjectId()));

                                    // Notify players that were controlling the object that object was removed.
                                    sendTo(EGEPacket::generateSSceneObjectControl(nullptr), [object](ClientConnection* client)->bool {
                                            EGEClientConnection* egeClient = (EGEClientConnection*)client;
                                            if(egeClient->getControlledSceneObject() == object->getObjectId())
                                            {
                                                std::cerr << "EGEServer: Notifying " << egeClient << " about deletion of controller for " << object->getObjectId() << std::endl;
                                                return true;
                                            }
                                            return false;
                                           });
                                });
    EGEGame::setScene(scene);
}

EventResult EGEServer::onReceive(ClientConnection* client, std::shared_ptr<Packet> packet)
{
    sf::Lock lock(m_clientsAccessMutex);
    EGEPacket* egePacket = (EGEPacket*)packet.get();

    /*if constexpr(EGESERVER_DEBUG)
    {
        sf::Packet sfPacket = egePacket->toSFMLPacket();
        std::cerr << "Server: Hex dump: " << std::endl;
        hexDump(sfPacket.getData(), sfPacket.getDataSize(), HexDumpSettings{8});
    }*/

    EGEClientConnection* egeClient = (EGEClientConnection*)client;

    if(PING_DEBUG && egeClient->wasPinged())
        std::cerr << "%%%%% Client is now responding. clearing ping flag %%%%%" << std::endl;

    egeClient->setLastRecvTime(EGE::Time(time(Time::Unit::Seconds), Time::Unit::Seconds));
    egeClient->setPinged(false);

    /*if constexpr(EGESERVER_DEBUG)
    {
        std::cerr << "Server: EGEPacket(" << EGEPacket::typeString(egePacket->getType()) << ") ";
        std::cerr << egePacket->getArgs()->toString() << std::endl;
    }*/

    switch(egePacket->getType())
    {
    case EGEPacket::Type::_Ping:
        egeClient->send(EGEPacket::generate_Pong());
        break;
    case EGEPacket::Type::_Pong:
        break;
    case EGEPacket::Type::_ProtocolVersion:
        {
            egeClient->send(EGEPacket::generate_ProtocolVersion(EGE_PROTOCOL_VERSION));
            int value = egePacket->getArgs()->getObject("value").lock()->asInt();
            if(value != EGE_PROTOCOL_VERSION)
            {
                std::cerr << "0021 EGE/egeNetwork: Client PROTOCOL_VERSION doesn't match server! (required "
                    << EGE_PROTOCOL_VERSION << ", got " << value << ")" << std::endl;
                return EventResult::Failure;
            }
            egeClient->send(EGEPacket::generate_Pong());
        }
        break;
    case EGEPacket::Type::CLogin:
        {
            return onLogin(egeClient, egePacket->getArgs());
        }
        break;
    case EGEPacket::Type::CSceneObjectControl:
        {
            std::shared_ptr<ObjectMap> args = egePacket->getArgs();
            auto id = args->getObject("id");
            ASSERT(!id.expired() && id.lock()->isInt());
            auto data = args->getObject("data");
            ASSERT(!data.expired() && data.lock()->isMap());
            auto data_map = data.lock()->asMap();
            auto data_name = data_map->getObject("type");
            ASSERT(!data_name.expired() && data_name.lock()->isString());
            auto data_args = data_map->getObject("args");
            ASSERT(!data_args.expired() && data_args.lock()->isMap());

            if(!getScene()) // cannot control object when no scene is created!
                return EventResult::Failure;

            auto controller = getController(id.lock()->asInt());
            controller->handleRequest(ControlObject(data_name.lock()->asString(), data_args.lock()->asMap()));
        }
        break;
    case EGEPacket::Type::CSceneObjectRequest:
        {
            std::shared_ptr<ObjectMap> args = egePacket->getArgs();
            auto id = args->getObject("id");
            ASSERT(!id.expired() && id.lock()->isInt());
            // TODO: syncObject();

            auto scene = getScene();
            if(!scene) // cannot request object when no scene is created!
                return EventResult::Failure;

            auto sceneObject = scene->getObject(id.lock()->asInt());
            if(!sceneObject) // object doesn't exist
                return EventResult::Failure;

            std::cerr << "so request " << id.lock()->asInt() << std::endl;
            egeClient->send(EGEPacket::generateSSceneObjectCreation(sceneObject, sceneObject->getId()));
            if(egeClient->getControlledSceneObject() == id.lock()->asInt())
                egeClient->send(EGEPacket::generateSSceneObjectControl(sceneObject));
        }
        break;
    default:
        std::cerr << "EGEClient::onReceive: not implemented packet handler: " + EGEPacket::typeString(egePacket->getType()) << std::endl;
        return EventResult::Failure;
    }

    return EventResult::Success;
}

EventResult EGEServer::onLoad()
{
    if(!EGEGame::initialize())
        return EventResult::Failure;

    // Run server thread
    auto serverNetworkWorker = [this]()->int {
        std::cerr << "001E EGE/egeNetwork: Starting server" << std::endl;
        if(!start())
            return 1;

        while(isRunning())
            select();

        return 0;
    };
    auto serverNetworkCallback = [this](AsyncTask::State state) {
        std::cerr << "001F EGE/egeNetwork: Closing server" << std::endl;
        exit(state.returnCode);
    };

    std::shared_ptr<AsyncTask> task = make<AsyncTask>(serverNetworkWorker, serverNetworkCallback);
    addAsyncTask(task, "EGEServer network task");

    return EventResult::Success;
}

void EGEServer::onExit(int exitCode)
{
    (void) exitCode;
    close();
}

void EGEServer::onTick(long long)
{
    sf::Lock lock(m_clientsAccessMutex);

    // Check if clients are alive.
    for(auto it: *this)
    {
        EGEClientConnection* client = (EGEClientConnection*)it.second.get();
        double _time = time(Time::Unit::Seconds);

        // If client is not talking to us, ping it to check if it's alive.
        if(_time > client->getLastRecvTime().getValue() + 3.0)
        {
            // If we pinged client, and it's not responding, kick it.

            if constexpr(PING_DEBUG)
            {
                std::cerr << "===== PING because of no recv in 3 second =====" << std::endl;
            }
            DUMP(PING_DEBUG, client->wasPinged());

            if(client->wasPinged())
            {
                if constexpr(PING_DEBUG) std::cerr << "===== Kicking. =====" << std::endl;
                //kickClientWithReason(client, "Timed out");
                client->setPinged(false);

                // FIXME: update `it' instead of giving up on one client !! :)
                break;
            }

            if constexpr(PING_DEBUG) std::cerr << "===== Sending _Ping. =====" << std::endl;
            client->send(EGEPacket::generate_Ping());

            // Remember that we pinged the client. The ping flag will be
            // cleared when network thread receives any data from client.
            client->setPinged();
            client->setLastRecvTime(EGE::Time(time(Time::Unit::Seconds), Time::Unit::Seconds));
        }
    }

    // Update scene, because it's not done in GameLoop.
    if(getScene())
        getScene()->onUpdate(getTickCount());

    // Send object updates to Client.
    // TODO: It will be better to not check every object!
    if(getScene())
    {
        for(auto object: *getScene())
        {
            auto sceneObject = object.second;

            if(sceneObject->getMainChangedFlag())
            {
                sceneObject->clearMainChangedFlag();
                sendToAll(EGEPacket::generateSSceneObjectUpdate_Main(sceneObject));
            }
            if(sceneObject->getExtendedChangedFlag())
            {
                sceneObject->clearExtendedChangedFlag();
                sendToAll(EGEPacket::generateSSceneObjectUpdate_Extended(sceneObject));
            }
        }
    }
}

EventResult EGEServer::onLogin(EGEClientConnection* client, std::shared_ptr<ObjectMap>)
{
    // Send SceneObject data to Client.
    auto scene = getScene();
    if(scene)
    {
        for(auto object: *scene)
        {
            bool success = client->send(EGEPacket::generateSSceneObjectCreation(object.second, object.second->getId()));
            if(!success)
                return EventResult::Failure;
        }
    }

    return EventResult::Success;
}

void EGEServer::kickClientWithReason(EGEClientConnection* client, std::string reason)
{
    ASSERT(client);
    client->send(EGEPacket::generateSDisconnectReason(reason));
    onClientDisconnect(client, reason);
    kickClient(client);
}

std::shared_ptr<ClientConnection> EGEServer::makeClient(Server* server, std::shared_ptr<sf::TcpSocket> socket)
{
    return std::shared_ptr<ClientConnection>((ClientConnection*)new EGEClientConnection((EGEServer*)server, socket));
}

void EGEServer::setDefaultController(EGEClientConnection* client, std::shared_ptr<SceneObject> sceneObject)
{
    ASSERT(client);
    ASSERT(sceneObject);
    std::cerr << "EGEServer: Setting default controller for " << client << ": " << sceneObject->getObjectId() << std::endl;
    client->setControlledSceneObject(sceneObject->getObjectId());
    client->send(EGEPacket::generateSSceneObjectControl(sceneObject));
}

std::shared_ptr<ServerNetworkController> EGEServer::getController(long long objectId)
{
    return m_controllersForObjects[objectId];
}

void EGEServer::control(std::shared_ptr<SceneObject> object, const ControlObject& data)
{
    ASSERT(object);
    auto controller = getController(object->getObjectId());
    ASSERT(controller);
    controller->handleRequest(data);
}

void EGEServer::requestControl(std::shared_ptr<SceneObject> object, const ControlObject& data)
{
    ASSERT(object);
    auto controller = getController(object->getObjectId());
    ASSERT(controller);
    controller->sendRequest(data);
}

}
