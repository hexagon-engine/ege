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
        EGEGame::setScene(scene);
        return;
    }

    scene->events<AddObjectEvent>().add([this](AddObjectEvent& event) {
        // Inform clients.
        sendToAll(EGEPacket::generateSSceneObjectCreation(event.object, event.object.getType()->getId()));
        return EventResult::Success;
    });

    scene->events<RemoveObjectEvent>().add([this](RemoveObjectEvent& event) {
        // Inform clients.
        sendToAll(EGEPacket::generateSSceneObjectDeletion(event.object.getObjectId()));

        // Find all clients that controlled this object and remove them controllers.
        forEachClientIf([&](auto& client) { return client.canControl(event.object); }, [&](auto& client) {
            if(client.getControlledSceneObject() == &event.object)
                client.setControlledSceneObject(nullptr);
            if(client.hasAdditionalController(event.object))
                client.removeAdditionalController(event.object);
        });
        return EventResult::Success;
    });

    EGEGame::setScene(scene);
}

void EGEServer::onReceive(ClientConnection& client, Packet const& packet)
{
    if(PING_DEBUG && client.wasPinged())
        ege_log.debug() << "%%%%% Client is now responding. clearing ping flag %%%%%";

    client.setLastRecvTime(EGE::Time(time(Time::Unit::Seconds), Time::Unit::Seconds));
    client.setPinged(false);

    auto type = packet.getType();
    auto args = packet.getArgs();

    ege_log.info() << EGEPacket::typeString(type) << " from " << client.toString();

    switch(type)
    {
    case EGEPacket::Type::_Ping:
        client.send(EGEPacket::generate_Pong());
        break;
    case EGEPacket::Type::_Pong:
        break;
    case EGEPacket::Type::_ProtocolVersion:
        {
            int value = packet.getArgs()->getObject("value").asInt().valueOr(0);
            if(value != EGE_PROTOCOL_VERSION)
            {
                String message = "Invalid protocol version (need " + std::to_string(EGE_PROTOCOL_VERSION) + ", got " + std::to_string(value) + ")";
                ege_log.debug() << message;
                client.disconnectWithReason(message);
                return;
            }
            client.setProtVerCheckSuccess();
        }
        break;
    case EGEPacket::Type::CLogin:
        {
            // Check if client login credentials are valid.
            if(!acceptLoginData(client, args))
            {
                client.disconnectWithReason("Login failed");
                return;
            }

            // Send SceneObject data to Client.
            auto scene = getScene();
            if(scene)
            {
                for(auto object: *scene)
                {
                    client.send(EGEPacket::generateSSceneObjectCreation(*object.second, object.second->getType()->getId()));
                }
            }

            // Inform derived class
            onLogin(client);
        }
        break;
    case EGEPacket::Type::CSceneObjectControl:
        {
            SharedPtr<ObjectMap> args = packet.getArgs();

            auto id = args->getObject("id").asInt();
            if(!id.hasValue())
            {
                client.disconnectWithReason("CSceneObjectControl id not given");
                return;
            }

            auto data = args->getObject("data").to<ObjectMap>();
            if(!data.hasValue())
            {
                client.disconnectWithReason("CSceneObjectControl data not given");
                return;
            }

            auto data_name = data.value()->getObject("type").asString();
            if(!data_name.hasValue())
            {
                client.disconnectWithReason("CSceneObjectControl type not given");
                return;
            }

            auto data_args = data.value()->getObject("args").to<ObjectMap>();
            if(!data_args.hasValue())
            {
                client.disconnectWithReason("CSceneObjectControl args not given");
                return;
            }

            auto scene = getScene();
            if(!scene) // cannot control object when no scene is created!
            {
                client.disconnectWithReason("CSceneObjectControl nonexisting scene");
                return;
            }

            if constexpr(EGESERVER_DEBUG)
                ege_log.debug() << "EGEServer: Trying to control object: " <<  id.value();

            auto sceneObject = scene->getObject(id.value());
            if(!sceneObject)
            {
                client.disconnectWithReason("CSceneObjectControl on nonexisting object");
                return;
            }

            // Check permissions
            if(!client.canControl(*sceneObject))
            {
                client.disconnectWithReason("CSceneObjectControl without permission: " + std::to_string(id.value()));
                return;
            }

            runController(*sceneObject, ControlPacket(data_name.value(), data_args.value()));
        }
        break;
    case EGEPacket::Type::CSceneObjectRequest:
        {
            SharedPtr<ObjectMap> args = packet.getArgs();
            auto id = args->getObject("id").asInt();
            if(!id.hasValue())
            {
                client.disconnectWithReason("CSceneObjectRequest id not given");
                return;
            }

            auto scene = getScene();
            if(!scene) // cannot request object when no scene is created!
            {
                client.disconnectWithReason("CSceneObjectRequest on nonexisting scene");
                return;
            }

            auto sceneObject = scene->getObject(id.value());
            if(!sceneObject) // object doesn't exist
            {
                // It's ok.
                client.send(EGEPacket::generateSSceneObjectDeletion(sceneObject->getObjectId()));
                return;
            }

            ege_log.debug() << "EGEServer: SceneObject requested: " << id.value();
            client.send(EGEPacket::generateSSceneObjectCreation(*sceneObject, sceneObject->getType()->getId()));
            if(client.getControlledSceneObject() == sceneObject.get())
                client.send(EGEPacket::generateSDefaultControllerId(sceneObject.get()));
            if(client.hasAdditionalController(*sceneObject))
                client.send(EGEPacket::generateSAdditionalControllerId(*sceneObject, true));
        }
        break;
    case EGEPacket::Type::_Version:
        {
            int value = packet.getArgs()->getObject("value").asInt().valueOr(0);
            String str = packet.getArgs()->getObject("string").asString().valueOr("Generic EGE::EGEClient");

            if(value != getVersion())
            {
                ege_log.error() << "EGEServer: Invalid client version! (need " << getVersion() << ", got " << value << ")";
                client.disconnectWithReason("Invalid client version");
                return;
            }

            if(str != getVersionString())
            {
                ege_log.error() << "EGEServer: Invalid client! (need '" << getVersionString() << "', got '" << str << "')";
                client.disconnectWithReason("Invalid client");
                return;
            }

            client.setAgentVerCheckSuccess();
        }
        break;
    default:
        ege_log.error() << "EGEServer: Unimplemented packet handler: " + EGEPacket::typeString(packet.getType());
        client.disconnect();
    }
}

void EGEServer::onTick(TickCount)
{
    // Check if clients are alive.
    for(auto& client: clients())
    {
        double _time = time(Time::Unit::Seconds);

        // Version check timeout
        if((!client->agentVerCheckSucceeded() || !client->protVerCheckSucceeded())
        && time(Time::Unit::Seconds) - client->getCreateTime().getValue() > 10)
        {
            ege_log.error() << "EGEServer: Version check timed out for client " << client->toString();
            client->disconnect();
            return;
        }

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
                client->disconnectWithReason("Timed out");
                //client->setPinged(false);

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

void EGEServer::control(SceneObject const& object, String action, SharedPtr<ObjectMap> data)
{
    sendToAll(EGEPacket::generateSSceneObjectControl(object, {action, data}));
}

}
