/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "EGEServer.h"

#include "EGEClientConnection.h"
#include "EGEPacket.h"

#include <ege/asyncLoop/AsyncTask.h>
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
    bool success = true;

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
    scene->setAddObjectCallback([this](std::shared_ptr<SceneObject> object) {
                                    sendToAll(EGEPacket::generateSSceneObjectCreation(object, object->getId()));
                                });
    scene->setRemoveObjectCallback([this](std::shared_ptr<SceneObject> object) {

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
        std::cerr << "%%%%% Client is now responsing. clearing ping flag %%%%%" << std::endl;

    egeClient->setLastRecvTime(EGE::Time(time(Time::Unit::Seconds), Time::Unit::Seconds));
    egeClient->setPinged(false);

    if constexpr(EGESERVER_DEBUG)
    {
        std::cerr << "Server: Received EGEPacket, type=" << EGEPacket::typeString(egePacket->getType()) << std::endl;
        std::cerr << "args=" << egePacket->getArgs()->toString() << std::endl;
    }

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

    std::shared_ptr<AsyncTask> task = std::make_shared<AsyncTask>(serverNetworkWorker, serverNetworkCallback);
    addAsyncTask(task, "EGEServer network task");

    return EventResult::Success;
}

void EGEServer::onExit(int exitCode)
{
    (void) exitCode;
    close();
}

void EGEServer::onTick(long long tickCount)
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
                kickClientWithReason(client, "Timed out");

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

}
