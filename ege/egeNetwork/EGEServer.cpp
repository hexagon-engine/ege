/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*/

#include "EGEServer.h"

#include "EGEClientConnection.h"
#include "EGEPacket.h"

#include <ege/asyncLoop/AsyncTask.h>
#include <ege/controller/ControlPacket.h>
#include <ege/debug/Dump.h>
#include <ege/debug/Logger.h>
#include <ege/network/ClientConnection.h>
#include <iomanip>
#include <iostream>

namespace EGE
{

EventResult EGEServer::onClientConnect(ClientConnection& client)
{
    sf::Lock lock(m_clientsAccessMutex);

    // FIXME: should it be asynchronous?
    if(!client.send(EGEPacket::generate_Ping()))
        return EventResult::Failure;
    if(!client.send(EGEPacket::generate_ProtocolVersion(EGE_PROTOCOL_VERSION)))
        return EventResult::Failure;
    if(!client.send(EGEPacket::generate_Version(getVersion(), getVersionString())))
        return EventResult::Failure;
    if(!client.send(EGEPacket::generateSLoginRequest(getLoginData(client))))
        return EventResult::Failure;

    return EventResult::Success;
}

EventResult EGEServer::onClientDisconnect(ClientConnection& client)
{
    sf::Lock lock(m_clientsAccessMutex);
    if(!client.send(EGEPacket::generateSDisconnectReason("Disconnected")))
        return EventResult::Failure;

    return EventResult::Success;
}

void EGEServer::setScene(SharedPtr<Scene> scene)
{
    if(!scene)
    {
        m_controllersForObjects.clear();
        EGEGame::setScene(scene);
        return;
    }

    scene->events<AddObjectEvent>().add([this](AddObjectEvent& event) {
        // Add controller to controller map.
        m_controllersForObjects[event.object.getObjectId()] = makeController(event.object);
        sendToAll(EGEPacket::generateSSceneObjectCreation(event.object, event.object.getType()->getId()));
        return EventResult::Success;
    });

    scene->events<RemoveObjectEvent>().add([this](RemoveObjectEvent& event) {
        // Remove controller from controller map.
        auto it = m_controllersForObjects.find(event.object.getObjectId());
        if(it != m_controllersForObjects.end())
            m_controllersForObjects.erase(it);

        sendToAll(EGEPacket::generateSSceneObjectDeletion(event.object.getObjectId()));

        // Notify players that were controlling the object that object was removed.
        sendTo(EGEPacket::generateSDefaultControllerId(nullptr), [event](ClientConnection& client)->bool {
            EGEClientConnection& egeClient = (EGEClientConnection&)client;
            if(egeClient.getControlledSceneObject() == event.object.getObjectId())
            {
                err(LogLevel::Verbose) << "EGEServer: Notifying " << egeClient.getID() << " about deletion of controller for " << event.object.getObjectId();
                return true;
            }
            return false;
        });
        return EventResult::Success;
    });

    EGEGame::setScene(scene);
}

EventResult EGEServer::onReceive(ClientConnection& client, SharedPtr<Packet> packet)
{
    sf::Lock lock(m_clientsAccessMutex);
    EGEPacket* egePacket = (EGEPacket*)packet.get();

    if constexpr(EGEPACKET_DEBUG)
    {
        std::cerr << "Server: Received packet (" << EGEPacket::typeString(egePacket->getType()) << ")" << std::endl;
        printObject(egePacket->getArgs());
    }

    EGEClientConnection& egeClient = (EGEClientConnection&)client;

    if(PING_DEBUG && egeClient.wasPinged())
        std::cerr << "%%%%% Client is now responding. clearing ping flag %%%%%" << std::endl;

    egeClient.setLastRecvTime(EGE::Time(time(Time::Unit::Seconds), Time::Unit::Seconds));
    egeClient.setPinged(false);

    // Version check
    if((!egeClient.agentVerCheckSucceeded() || !egeClient.protVerCheckSucceeded())
    && time(Time::Unit::Seconds) - egeClient.getCreateTime().getValue() > 10)
    {
        log(LogLevel::Error) << "Version check timed out for client " << egeClient.getID();
        return EventResult::Failure;
    }

    switch(egePacket->getType())
    {
    case EGEPacket::Type::_Data:
        onData(egeClient, egePacket->getArgs());
        break;
    case EGEPacket::Type::_Ping:
        egeClient.send(EGEPacket::generate_Pong());
        break;
    case EGEPacket::Type::_Pong:
        break;
    case EGEPacket::Type::_ProtocolVersion:
        {
            egeClient.send(EGEPacket::generate_ProtocolVersion(EGE_PROTOCOL_VERSION));
            int value = egePacket->getArgs()->getObject("value").asInt().valueOr(0);
            if(value != EGE_PROTOCOL_VERSION)
            {
                err(LogLevel::Error) << "0021 EGE/egeNetwork: Client PROTOCOL_VERSION doesn't match server! (required "
                    << EGE_PROTOCOL_VERSION << ", got " << value << ")";
                kickClientWithReason(egeClient, "Invalid protocol version (need " + std::to_string(EGE_PROTOCOL_VERSION) + ", got " + std::to_string(value) + ")");
                return EventResult::Failure;
            }
            egeClient.send(EGEPacket::generate_Pong());
            egeClient.setProtVerCheckSuccess();
        }
        break;
    case EGEPacket::Type::CLogin:
        {
            return onLogin(egeClient, egePacket->getArgs());
        }
        break;
    case EGEPacket::Type::CSceneObjectControl:
        {
            SharedPtr<ObjectMap> args = egePacket->getArgs();

            auto id = args->getObject("id").asInt();
            if(!id.hasValue())
                return EventResult::Failure;

            auto data = args->getObject("data").to<ObjectMap>();
            if(!data.hasValue())
                return EventResult::Failure;

            auto data_name = data.value()->getObject("type").asString();
            if(!data_name.hasValue())
                return EventResult::Failure;

            auto data_args = data.value()->getObject("args").to<ObjectMap>();
            if(!data_args.hasValue())
                return EventResult::Failure;

            if(!getScene()) // cannot control object when no scene is created!
                return EventResult::Failure;

            if constexpr(EGESERVER_DEBUG)
                log() << "Trying to control object: " <<  id.value();

            auto controller = getController(id.value());

            if(!canControlPacket(*controller, egeClient))
            {
                err() << "Client " << egeClient.getID() << " tried to use controller with ID " << id.value() << " without permission!";
                return EventResult::Failure;
            }

            if(controller)
                controller->handleRequest(ControlPacket(data_name.value(), data_args.value()));
            else
                return EventResult::Failure; // kick f*****g cheaters
        }
        break;
    case EGEPacket::Type::CSceneObjectRequest:
        {
            SharedPtr<ObjectMap> args = egePacket->getArgs();
            auto id = args->getObject("id").asInt();
            if(!id.hasValue())
                return EventResult::Failure;

            auto scene = getScene();
            if(!scene) // cannot request object when no scene is created!
                return EventResult::Failure;

            auto sceneObject = scene->getObject(id.value());
            if(!sceneObject) // object doesn't exist
                return EventResult::Failure;

            err(LogLevel::Debug) << "SceneObject requested: " << id.value();
            egeClient.send(EGEPacket::generateSSceneObjectCreation(*sceneObject, sceneObject->getType()->getId()));
            if(egeClient.getControlledSceneObject() == id.value())
                egeClient.send(EGEPacket::generateSDefaultControllerId(sceneObject.get()));
        }
        break;
    case EGEPacket::Type::_Version:
        {
            int value = egePacket->getArgs()->getObject("value").asInt().valueOr(0);
            String str = egePacket->getArgs()->getObject("string").asString().valueOr("Generic EGE::EGEClient");

            if(value != getVersion())
            {
                err() << "Invalid server version! (need " << getVersion() << ", got " << value << ")";
                return EventResult::Failure;
            }

            if(str != getVersionString())
            {
                err() << "Invalid server! (need '" << getVersionString() << "', got '" << str << "')";
                return EventResult::Failure;
            }

            egeClient.setAgentVerCheckSuccess();
        }
        break;
    default:
        err(LogLevel::Error) << "EGEClient::onReceive: not implemented packet handler: " + EGEPacket::typeString(egePacket->getType());
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
        err(LogLevel::Info) << "001E EGE/egeNetwork: Starting server";
        log(LogLevel::Notice) << "Extendable Game Engine egeNetwork Server v" << EGE_PROTOCOL_VERSION;
        log(LogLevel::Notice) << "Agent: " << getVersionString() << " v" << getVersion();
        if(!start())
            return 1;

        while(isRunning())
            select();

        return 0;
    };
    auto serverNetworkCallback = [this](AsyncTask::State state) {
        err(LogLevel::Info) << "001F EGE/egeNetwork: Closing server";
        exit(state.returnCode);
    };

    SharedPtr<AsyncTask> task = make<AsyncTask>(serverNetworkWorker, serverNetworkCallback);
    addAsyncTask(task, "EGEServer network task");

    return EventResult::Success;
}

void EGEServer::onExit(int exitCode)
{
    (void) exitCode;
    close();
}

void EGEServer::onTick(TickCount)
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
        }bool canControlPacket(SharedPtr<SceneObject> object, EGEClientConnection* client);
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
                sendToAll(EGEPacket::generateSSceneObjectUpdate_Main(*sceneObject));
            }
            if(sceneObject->getExtendedChangedFlag())
            {
                sceneObject->clearExtendedChangedFlag();
                sendToAll(EGEPacket::generateSSceneObjectUpdate_Extended(*sceneObject));
            }
        }
    }
}

EventResult EGEServer::onLogin(EGEClientConnection& client, SharedPtr<ObjectMap>)
{
    // Send SceneObject data to Client.
    auto scene = getScene();
    if(scene)
    {
        for(auto object: *scene)
        {
            bool success = client.send(EGEPacket::generateSSceneObjectCreation(*object.second, object.second->getType()->getId()));
            if(!success)
                return EventResult::Failure;
        }
    }

    return EventResult::Success;
}

void EGEServer::kickClientWithReason(EGEClientConnection& client, std::string reason)
{
    client.send(EGEPacket::generateSDisconnectReason(reason));
    onClientDisconnect(client, reason);
    kickClient(client);
}

SharedPtr<ClientConnection> EGEServer::makeClient(Server& server, SharedPtr<sf::TcpSocket> socket)
{
    return make<EGEClientConnection>((EGEServer&)server, socket);
}

void EGEServer::setDefaultController(EGEClientConnection& client, SceneObject* sceneObject)
{
    client.setControlledSceneObject(sceneObject ? sceneObject->getObjectId() : 0);
    client.send(EGEPacket::generateSDefaultControllerId(sceneObject));
}

SharedPtr<ServerNetworkController> EGEServer::getController(UidType objectId)
{
    return m_controllersForObjects[objectId];
}

void EGEServer::control(SceneObject& object, const ControlPacket& data)
{
    auto controller = getController(object.getObjectId());
    ASSERT(controller);
    controller->handleRequest(data);
}

void EGEServer::requestControl(SceneObject& object, const ControlPacket& data)
{
    auto controller = getController(object.getObjectId());
    ASSERT(controller);
    controller->sendRequest(data);
}

bool EGEServer::canControlPacket(ServerNetworkController& controller, EGEClientConnection& client)
{
    auto& so = (SceneObject&)controller.getObject();
    return so.getObjectId() == client.getControlledSceneObject() || client.hasAdditionalController(so.getObjectId());
}

void EGEServer::addAdditionalController(EGEClientConnection& client, SceneObject& sceneObject)
{
    client.addAdditionalController(sceneObject.getObjectId());
    client.send(EGEPacket::generateSAdditionalControllerId(sceneObject, false));
}

void EGEServer::removeAdditionalController(EGEClientConnection& client, SceneObject& sceneObject)
{
    client.removeAdditionalController(sceneObject.getObjectId());
    client.send(EGEPacket::generateSAdditionalControllerId(sceneObject, true));
}

}
