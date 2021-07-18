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

#include <ege/controller/ControlPacket.h>
#include <ege/core/AsyncTask.h>
#include <ege/debug/Dump.h>
#include <ege/debug/Logger.h>
#include <iomanip>
#include <iostream>

namespace EGE
{

void EGEServer::onClientConnect(ClientConnection& client)
{
    // FIXME: should it be asynchronous?
    client.send(EGEPacket::generate_Ping());
    client.send(EGEPacket::generate_ProtocolVersion(EGE_PROTOCOL_VERSION));
    client.send(EGEPacket::generate_Version(getVersion(), getVersionString()));
    client.send(EGEPacket::generateSLoginRequest(getLoginData(client)));
}

void EGEServer::onClientDisconnect(ClientConnection& client)
{
    client.send(EGEPacket::generateSDisconnectReason("Disconnected"));
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
        sendToIf(EGEPacket::generateSDefaultControllerId(nullptr), [event](ClientConnection& client) {
            if(client.getControlledSceneObject() == event.object.getObjectId())
            {
                ege_log.verbose() << "EGEServer: Notifying " << client.toString() << " about deletion of controller for " << event.object.getObjectId();
                return true;
            }
            return false;
        });
        return EventResult::Success;
    });

    EGEGame::setScene(scene);
}

void EGEServer::onReceive(ClientConnection& client, Packet const& packet)
{
    if constexpr(EGEPACKET_DEBUG)
    {
        ege_log.debug() << "EGEServer: Received packet (" << EGEPacket::typeString(packet.getType()) << ")";
        printObject(packet.getArgs());
    }

    EGEClientConnection& egeClient = (EGEClientConnection&)client;

    if(PING_DEBUG && egeClient.wasPinged())
        ege_log.debug() << "%%%%% Client is now responding. clearing ping flag %%%%%";

    egeClient.setLastRecvTime(EGE::Time(time(Time::Unit::Seconds), Time::Unit::Seconds));
    egeClient.setPinged(false);

    // Version check
    if((!egeClient.agentVerCheckSucceeded() || !egeClient.protVerCheckSucceeded())
    && time(Time::Unit::Seconds) - egeClient.getCreateTime().getValue() > 10)
    {
        ege_log.error() << "EGEServer: Version check timed out for client " << egeClient.toString();
        client.disconnect();
        return;
    }

    switch(packet.getType())
    {
    case EGEPacket::Type::_Data:
        onData(egeClient, packet.getArgs());
        break;
    case EGEPacket::Type::_Ping:
        egeClient.send(EGEPacket::generate_Pong());
        break;
    case EGEPacket::Type::_Pong:
        break;
    case EGEPacket::Type::_ProtocolVersion:
        {
            egeClient.send(EGEPacket::generate_ProtocolVersion(EGE_PROTOCOL_VERSION));
            int value = packet.getArgs()->getObject("value").asInt().valueOr(0);
            if(value != EGE_PROTOCOL_VERSION)
            {
                String message = "Invalid protocol version (need " + std::to_string(EGE_PROTOCOL_VERSION) + ", got " + std::to_string(value) + ")";
                ege_log.debug() << message;
                kickClientWithReason(egeClient, message);
                return;
            }
            egeClient.send(EGEPacket::generate_Pong());
            egeClient.setProtVerCheckSuccess();
        }
        break;
    case EGEPacket::Type::CLogin:
        {
            if(onLogin(egeClient, packet.getArgs()) == EventResult::Failure)
            {
                client.disconnect();
                return;
            }
        }
        break;
    case EGEPacket::Type::CSceneObjectControl:
        {
            SharedPtr<ObjectMap> args = packet.getArgs();

            auto id = args->getObject("id").asInt();
            if(!id.hasValue())
            {
                client.disconnect();
                return;
            }

            auto data = args->getObject("data").to<ObjectMap>();
            if(!data.hasValue())
            {
                client.disconnect();
                return;
            }

            auto data_name = data.value()->getObject("type").asString();
            if(!data_name.hasValue())
            {
                client.disconnect();
                return;
            }

            auto data_args = data.value()->getObject("args").to<ObjectMap>();
            if(!data_args.hasValue())
            {
                client.disconnect();
                return;
            }

            if(!getScene()) // cannot control object when no scene is created!
            {
                client.disconnect();
                return;
            }

            if constexpr(EGESERVER_DEBUG)
                ege_log.debug() << "EGEServer: Trying to control object: " <<  id.value();

            auto controller = getController(id.value());

            if(!controller)
            {
                client.disconnect();
                return;
            } // kick f*****g cheaters

            if(!canControlPacket(*controller, egeClient))
            {
                ege_log.error() << "EGEServer: Client " << egeClient.toString() << " tried to use controller with ID " << id.value() << " without permission!";
                client.disconnect();
                return;
            }

            controller->handleRequest(ControlPacket(data_name.value(), data_args.value()));
        }
        break;
    case EGEPacket::Type::CSceneObjectRequest:
        {
            SharedPtr<ObjectMap> args = packet.getArgs();
            auto id = args->getObject("id").asInt();
            if(!id.hasValue())
            {
                client.disconnect();
                return;
            }

            auto scene = getScene();
            if(!scene) // cannot request object when no scene is created!
            {
                client.disconnect();
                return;
            }

            auto sceneObject = scene->getObject(id.value());
            if(!sceneObject) // object doesn't exist
            {
                client.disconnect();
                return;
            }

            ege_log.debug() << "SceneObject requested: " << id.value();
            egeClient.send(EGEPacket::generateSSceneObjectCreation(*sceneObject, sceneObject->getType()->getId()));
            if(egeClient.getControlledSceneObject() == id.value())
                egeClient.send(EGEPacket::generateSDefaultControllerId(sceneObject.get()));
        }
        break;
    case EGEPacket::Type::_Version:
        {
            int value = packet.getArgs()->getObject("value").asInt().valueOr(0);
            String str = packet.getArgs()->getObject("string").asString().valueOr("Generic EGE::EGEClient");

            if(value != getVersion())
            {
                ege_log.error() << "EGEServer: Invalid client version! (need " << getVersion() << ", got " << value << ")";
                client.disconnect();
                return;
            }

            if(str != getVersionString())
            {
                ege_log.error() << "EGEServer: Invalid client! (need '" << getVersionString() << "', got '" << str << "')";
                client.disconnect();
                return;
            }

            egeClient.setAgentVerCheckSuccess();
        }
        break;
    default:
        ege_log.error() << "EGEServer: Unimplemented packet handler: " + EGEPacket::typeString(packet.getType());
        client.disconnect();
    }
}

EventResult EGEServer::onLoad()
{
    ege_log.notice() << "Extendable Game Engine egeNetwork Server v" << EGE_PROTOCOL_VERSION;
    ege_log.notice() << "Agent: " << getVersionString() << " v" << getVersion();
    if(!EGEGame::initialize())
        return EventResult::Failure;

    if(!listen(m_port))
        return EventResult::Failure;

    return EventResult::Success;
}

void EGEServer::onTick(TickCount tickCount)
{
    // Check if clients are alive.
    for(auto& client: clients())
    {
        double _time = time(Time::Unit::Seconds);

        // If client is not talking to us, ping it to check if it's alive.
        if(_time > client->getLastRecvTime().getValue() + 3.0)
        {
            // If we pinged client, and it's not responding, kick it.

            if constexpr(PING_DEBUG)
            {
                ege_log.debug() << "===== PING because of no recv in 3 second =====";
            }
            DUMP(PING_DEBUG, client->wasPinged());

            if(client->wasPinged())
            {
                if constexpr(PING_DEBUG) ege_log.debug() << "===== Kicking. =====";
                //kickClientWithReason(client, "Timed out");
                client->setPinged(false);

                // FIXME: update `it' instead of giving up on one client !! :)
                break;
            }

            if constexpr(PING_DEBUG) ege_log.debug() << "===== Sending _Ping. =====";
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
    client.disconnect();
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
