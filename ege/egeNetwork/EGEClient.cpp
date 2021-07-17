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

#include "EGEClient.h"

#include "EGEPacket.h"

#include <ege/core/AsyncTask.h>
#include <ege/debug/Dump.h>
#include <ege/debug/Logger.h>
#include <iomanip>
#include <iostream>

namespace EGE
{

EGEClient::~EGEClient()
{
    disconnect();
}

bool EGEClient::sendWithUID(EGEPacket const& packet)
{
    // FIXME: some assertion?
    UidType uid = packet.getArgs()->getObject("uid").asInt().valueOr(0);
    m_uidMap[uid] = packet.getType();
    return send(packet);
}

void EGEClient::onReceive(Packet const& packet)
{
    if constexpr(EGEPACKET_DEBUG)
    {
        ege_log.debug() << "EGEClient: Received packet (" << EGEPacket::typeString(packet.getType()) << ")";
        printObject(packet.getArgs());
    }

    switch(packet.getType())
    {
    case EGEPacket::Type::_Data:
        onData(packet.getArgs());
        break;
    case EGEPacket::Type::_Ping:
        send(EGEPacket::generate_Pong());
        break;
    case EGEPacket::Type::_Pong:
        break;
    case EGEPacket::Type::_ProtocolVersion:
        {
            int value = packet.getArgs()->getObject("value").asInt().valueOr(0);
            if(value != EGE_PROTOCOL_VERSION)
            {
                ege_log.error() << "EGEClient: Server PROTOCOL_VERSION doesn't match client! (required "
                    << EGE_PROTOCOL_VERSION << ", got " << value << ")";
                disconnect();
                return;
            }
            send(EGEPacket::generate_Pong());
        }
        break;
    case EGEPacket::Type::SLoginRequest:
        {
            SharedPtr<ObjectMap> args = packet.getArgs();
            sendWithUID(EGEPacket::generateCLogin(getLoginData(args)));
        }
        break;
    case EGEPacket::Type::SDisconnectReason:
        {
            SharedPtr<ObjectMap> args = packet.getArgs();
            onDisconnect(args->getObject("message").asString().valueOr("Disconnected"));
            disconnect();
        }
        break;
    case EGEPacket::Type::SSceneObjectCreation:
        {
            SharedPtr<ObjectMap> args = packet.getArgs();

            auto object = args->getObject("object").to<ObjectMap>();
            if(!object.hasValue())
            {
                disconnect();
                return;
            }

            auto id = args->getObject("id").asInt();
            if(!id.hasValue())
            {
                disconnect();
                return;
            }

            auto typeId = args->getObject("typeId").asString();
            if(!typeId.hasValue())
            {
                disconnect();
                return;
            }

            if(createSceneObjectFromData(object.value(), id.value(), typeId.value()) == EventResult::Failure)
            {
                disconnect();
                return;
            }
        }
        break;
    case EGEPacket::Type::SSceneObjectUpdate:
        {
            SharedPtr<ObjectMap> args = packet.getArgs();

            auto object = args->getObject("object").to<ObjectMap>();
            if(!object.hasValue())
            {
                disconnect();
                return;
            }

            auto id = args->getObject("id").asInt();
            if(!id.hasValue())
            {
                disconnect();
                return;
            }

            if(updateSceneObjectFromData(object.value(), id.value()) == EventResult::Failure)
            {
                disconnect();
                return;
            }
        }
        break;
    case EGEPacket::Type::SSceneObjectDeletion:
        {
            SharedPtr<ObjectMap> args = packet.getArgs();

            auto id = args->getObject("id").asInt();
            if(!id.hasValue())
            {
                disconnect();
                return;
            }

            auto scene = getScene();
            if(!scene) //scene not created
            {
                disconnect();
                return;
            }

            auto sceneObject = scene->getObject(id.value());
            if(!sceneObject) // Yay! We have predicted that the object will be removed! [or bugged server :)]
                return;

            sceneObject->setDead();
        }
        break;
    case EGEPacket::Type::SDefaultControllerId:
        {
            SharedPtr<ObjectMap> args = packet.getArgs();

            auto id = args->getObject("id").asInt();
            if(!id.hasValue())
            {
                disconnect();
                return;
            }

            auto scene = getScene();
            if(!scene) //scene not created
            {
                disconnect();
                return;
            }

            MaxInt _id = id.value();
            if(_id)
            {
                //std::cerr << "SDefaultControllerId " << _id << std::endl;
                auto sceneObject = scene->getObject(_id);
                if(!sceneObject) // Object was not yet created on client :(
                {
                    if constexpr(EGECLIENT_DEBUG) ege_log.debug() << "EGEClient: Sending requestObject from SDefaultControllerId handler";
                    DUMP(EGECLIENT_DEBUG, m_requestedObjects.count(_id));
                    if(!m_requestedObjects.count(_id))
                        requestObject(_id);
                    return;
                }
                m_defaultController = getController(_id);
                m_defaultController->onSetDefault();
                ASSERT(m_defaultController);
            }
        }
        break;
    case EGEPacket::Type::SSceneObjectControl:
        {
            SharedPtr<ObjectMap> args = packet.getArgs();

            auto id = args->getObject("id").asInt();
            if(!id.hasValue())
            {
                disconnect();
                return;
            }

            auto data = args->getObject("data").to<ObjectMap>();
            if(!data.hasValue())
            {
                disconnect();
                return;
            }

            auto data_name = data.value()->getObject("type").asString();
            if(!data_name.hasValue())
            {
                disconnect();
                return;
            }

            auto data_args = data.value()->getObject("args").to<ObjectMap>();
            if(!data_args.hasValue())
            {
                disconnect();
                return;
            }

            if(!getScene()) // cannot control object when no scene is created!
            {
                disconnect();
                return;
            }

            auto controller = getController(id.value());

            if(controller)
                controller->handleRequest(ControlPacket(data_name.value(), data_args.value()));
        }
        break;
    case EGEPacket::Type::_Version:
        {
            int value = packet.getArgs()->getObject("value").asInt().valueOr(0);
            String str = packet.getArgs()->getObject("string").asString().valueOr("Generic EGE::EGEServer");

            if(value != getVersion())
            {
                ege_log.error() << "EGEClient: Invalid server version! (need " << getVersion() << ", got " << value << ")";
                disconnect();
                return;
            }

            if(str != getVersionString())
            {
                ege_log.error() << "EGEClient: Invalid server! (need '" << getVersionString() << "', got '" << str << "')";
                disconnect();
                return;
            }

            // TODO: Force it on server-side and kick player when it doesn't send
            // its _Version packet
            send(EGEPacket::generate_Version(getVersion(), getVersionString()));
        }
        break;
    case EGEPacket::Type::SAdditionalControllerId:
        {
            UidType id = packet.getArgs()->getObject("id").asInt().valueOr(0);
            bool remove = packet.getArgs()->getObject("remove").asBoolean().valueOr(true);
            ege_log.debug() << "SAdditionalControllerId " << id << " " << remove;

            if(remove)
                removeAdditionalController(id);
            else
                addAdditionalController(id);
        }
        break;
    default:
        ege_log.error() << "EGEClient: Unimplemented packet handler: " + EGEPacket::typeString(packet.getType());
        disconnect();
    }
}

EventResult EGEClient::createSceneObjectFromData(SharedPtr<ObjectMap> object, UidType id, std::string typeId)
{
    if(!getScene()) //scene not created
        return EventResult::Success;

    SharedPtr<SceneObject> sceneObject = getScene()->createObject(typeId, object);
    if(!sceneObject)
        return EventResult::Failure;

    sceneObject->setObjectId(id);
    getScene()->addObject(sceneObject);

    return EventResult::Success;
}

EventResult EGEClient::updateSceneObjectFromData(SharedPtr<ObjectMap> object, UidType id)
{
    if(!getScene()) //scene not created
        return EventResult::Success;

    auto sceneObject = getScene()->getObject(id);

    if(!sceneObject)
    {
        if constexpr(EGECLIENT_DEBUG) ege_log.debug() << "EGEClient: Sending requestObject from update";
        DUMP(EGECLIENT_DEBUG, m_requestedObjects.count(id));
        if(!m_requestedObjects.count(id))
            requestObject(id);
        return EventResult::Success;
    }

    sceneObject->deserialize(object);
    return EventResult::Success;
}

void EGEClient::setScene(SharedPtr<Scene> scene)
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
        return EventResult::Success;
    });
    scene->events<RemoveObjectEvent>().add([this](RemoveObjectEvent& event) {
        // Remove controller from controller map.
        auto it = m_controllersForObjects.find(event.object.getObjectId());
        if(it != m_controllersForObjects.end())
            m_controllersForObjects.erase(it);
        return EventResult::Success;
    });
    EGEGame::setScene(scene);
}

EventResult EGEClient::onLoad()
{
    ege_log.notice() << "Extendable Game Engine egeNetwork Client v" << EGE_PROTOCOL_VERSION;
    ege_log.notice() << "Agent: " << getVersionString() << " v" << getVersion();

    if(!EGEGame::initialize())
        return EventResult::Failure;
    
    if(!connect(m_ip, m_port))
        return EventResult::Failure;

    if(!send(EGEPacket::generate_ProtocolVersion(EGE_PROTOCOL_VERSION)))
        return EventResult::Failure;

    return EventResult::Success;
}

void EGEClient::onExit(int exitCode)
{
    fire<ExitEvent>(exitCode);
}

SharedPtr<ClientNetworkController> EGEClient::getController(UidType objectId)
{
    return m_controllersForObjects[objectId];
}

SharedPtr<SceneObject> EGEClient::getDefaultControlledObject()
{
    return std::dynamic_pointer_cast<SceneObject>(getDefaultController());
}

SharedPtr<SceneObject> EGEClient::getControlledObject(UidType objectId)
{
    return std::dynamic_pointer_cast<SceneObject>(getController(objectId));
}

void EGEClient::control(SceneObject* object, const ControlPacket& data)
{
    if(!object)
    {
        if(m_defaultController)
            m_defaultController->handleRequest(data);

        return;
    }

    auto controller = getController(object->getObjectId());
    DUMP(EGECLIENT_DEBUG, m_requestedObjects.count(object->getObjectId()));
    if(!controller && !m_requestedObjects.count(object->getObjectId()))
    {
        if constexpr(EGECLIENT_DEBUG) ege_log.debug() << "EGEClient: Sending requestObject from control";
        requestObject(object->getObjectId());
    }
    controller->handleRequest(data);
}

void EGEClient::requestControl(SceneObject* object, const ControlPacket& data)
{
    if(!object)
    {
        if(m_defaultController)
            m_defaultController->sendRequest(data);

        return;
    }

    if(&m_defaultController->getObject() != object && !hasAdditionalController(object->getObjectId()))
    {
        ege_log.warning() << "Client has no additional or default controller for object " << object->getObjectId();
        return;
    }

    auto controller = getController(object->getObjectId());
    DUMP(EGECLIENT_DEBUG, m_requestedObjects.count(object->getObjectId()));
    if(!controller && !m_requestedObjects.count(object->getObjectId()))
    {
        if constexpr(EGECLIENT_DEBUG) ege_log.debug() << "EGEClient: Sending requestObject from requestControl";
        requestObject(object->getObjectId());
    }
    controller->sendRequest(data);

    // try to do it also on client...
    controller->handleRequest(data);
}

void EGEClient::requestObject(UidType id)
{
    if constexpr(EGECLIENT_DEBUG) ege_log.debug() << "EGEClient: Requesting object " << id << " from server";
    DUMP(EGECLIENT_DEBUG, m_requestedObjects.count(id));
    m_requestedObjects.insert(id);
    DUMP(EGECLIENT_DEBUG, m_requestedObjects.count(id));
    send(EGEPacket::generateCSceneObjectRequest(id));
}

}
