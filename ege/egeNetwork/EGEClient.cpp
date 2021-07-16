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

#include <ege/asyncLoop/AsyncTask.h>
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

bool EGEClient::sendWithUID(SharedPtr<EGEPacket> packet)
{
    // FIXME: some assertion?
    UidType uid = packet->getArgs()->getObject("uid").asInt().valueOr(0);
    m_uidMap[uid] = packet->getType();
    return send(packet);
}

EventResult EGEClient::onReceive(SharedPtr<Packet> packet)
{
    EGEPacket* egePacket = (EGEPacket*)packet.get();

    if constexpr(EGEPACKET_DEBUG)
    {
        std::cerr << "Client: Received packet (" << EGEPacket::typeString(egePacket->getType()) << ")" << std::endl;
        printObject(egePacket->getArgs());
    }

    switch(egePacket->getType())
    {
    case EGEPacket::Type::_Data:
        onData(egePacket->getArgs());
        break;
    case EGEPacket::Type::_Ping:
        send(EGEPacket::generate_Pong());
        break;
    case EGEPacket::Type::_Pong:
        break;
    case EGEPacket::Type::_ProtocolVersion:
        {
            int value = egePacket->getArgs()->getObject("value").asInt().valueOr(0);
            if(value != EGE_PROTOCOL_VERSION)
            {
                err() << "0020 EGE/egeNetwork: Server PROTOCOL_VERSION doesn't match client! (required "
                    << EGE_PROTOCOL_VERSION << ", got " << value << ")";
                return EventResult::Failure;
            }
            send(EGEPacket::generate_Pong());
        }
        break;
    case EGEPacket::Type::SLoginRequest:
        {
            SharedPtr<ObjectMap> args = egePacket->getArgs();
            sendWithUID(EGEPacket::generateCLogin(getLoginData(args)));
        }
        break;
    case EGEPacket::Type::SDisconnectReason:
        {
            SharedPtr<ObjectMap> args = egePacket->getArgs();
            onDisconnect(args->getObject("message").asString().valueOr("Disconnected"));
            disconnect();
        }
        break;
    case EGEPacket::Type::SSceneObjectCreation:
        {
            SharedPtr<ObjectMap> args = egePacket->getArgs();

            auto object = args->getObject("object").to<ObjectMap>();
            if(!object.hasValue())
                return EventResult::Failure;

            auto id = args->getObject("id").asInt();
            if(!id.hasValue())
                return EventResult::Failure;

            auto typeId = args->getObject("typeId").asString();
            if(!typeId.hasValue())
                return EventResult::Failure;

            return createSceneObjectFromData(object.value(), id.value(), typeId.value());
        }
        break;
    case EGEPacket::Type::SSceneObjectUpdate:
        {
            SharedPtr<ObjectMap> args = egePacket->getArgs();

            auto object = args->getObject("object").to<ObjectMap>();
            if(!object.hasValue())
                return EventResult::Failure;

            auto id = args->getObject("id").asInt();
            if(!id.hasValue())
                return EventResult::Failure;

            return updateSceneObjectFromData(object.value(), id.value());
        }
        break;
    case EGEPacket::Type::SSceneObjectDeletion:
        {
            SharedPtr<ObjectMap> args = egePacket->getArgs();

            auto id = args->getObject("id").asInt();
            if(!id.hasValue())
                return EventResult::Failure;

            auto scene = getScene();
            if(!scene) //scene not created
                return EventResult::Success;

            auto sceneObject = scene->getObject(id.value());
            if(!sceneObject) // Yay! We have predicted that the object will be removed! [or bugged server :)]
                return EventResult::Success;

            sceneObject->setDead();
        }
        break;
    case EGEPacket::Type::SDefaultControllerId:
        {
            SharedPtr<ObjectMap> args = egePacket->getArgs();

            auto id = args->getObject("id").asInt();
            if(!id.hasValue())
                return EventResult::Failure;

            auto scene = getScene();
            if(!scene) //scene not created
                return EventResult::Success;

            MaxInt _id = id.value();
            if(_id)
            {
                //std::cerr << "SDefaultControllerId " << _id << std::endl;
                auto sceneObject = scene->getObject(_id);
                if(!sceneObject) // Object was not yet created on client :(
                {
                    if constexpr(EGECLIENT_DEBUG) log(LogLevel::Debug) << "EGEClient: Sending requestObject from SDefaultControllerId handler";
                    DUMP(EGECLIENT_DEBUG, m_requestedObjects.count(_id));
                    if(!m_requestedObjects.count(_id))
                        requestObject(_id);
                    return EventResult::Success;
                }
                m_defaultController = getController(_id);
                m_defaultController->onSetDefault();
                ASSERT(m_defaultController);
            }
        }
        break;
    case EGEPacket::Type::SSceneObjectControl:
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


            auto controller = getController(id.value());

            if(controller)
                controller->handleRequest(ControlPacket(data_name.value(), data_args.value()));
        }
        break;
    case EGEPacket::Type::_Version:
        {
            int value = egePacket->getArgs()->getObject("value").asInt().valueOr(0);
            String str = egePacket->getArgs()->getObject("string").asString().valueOr("Generic EGE::EGEServer");

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

            // TODO: Force it on server-side and kick player when it doesn't send
            // its _Version packet
            send(EGEPacket::generate_Version(getVersion(), getVersionString()));
        }
        break;
    case EGEPacket::Type::SAdditionalControllerId:
        {
            UidType id = egePacket->getArgs()->getObject("id").asInt().valueOr(0);
            bool remove = egePacket->getArgs()->getObject("remove").asBoolean().valueOr(true);
            log() << "SAdditionalControllerId " << id << " " << remove;

            if(remove)
                removeAdditionalController(id);
            else
                addAdditionalController(id);
        }
        break;
    default:
        err() << "0022 EGE/egeNetwork: Unimplemented packet handler: " + EGEPacket::typeString(egePacket->getType());
        return EventResult::Failure;
    }

    return EventResult::Success;
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
        if constexpr(EGECLIENT_DEBUG) std::cerr << "EGEClient: Sending requestObject from update";
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
    if(!EGEGame::initialize())
        return EventResult::Failure;

    // Run client thread
    auto clientNetworkWorker = [this]()->int {
        log() << "001E EGE/egeNetwork: Starting client";
        log() << "Extendable Game Engine egeNetwork Client v" << EGE_PROTOCOL_VERSION;
        log() << "Agent: " << getVersionString() << " v" << getVersion();
        if(!connect(m_ip, m_port))
            return 1;

        send(EGEPacket::generate_ProtocolVersion(EGE_PROTOCOL_VERSION));

        while(isRunning())
        {
            update();
            if(!isConnected())
                exit(0);
        }

        return 0;
    };
    auto clientNetworkCallback = [this](AsyncTask::State state) {
        log() << "001F EGE/egeNetwork: Closing client";

        exit(state.returnCode);
        fire<ExitEvent>(state.returnCode);
    };

    m_clientTask = make<AsyncTask>(clientNetworkWorker, clientNetworkCallback);
    addAsyncTask(m_clientTask, "EGEClient network task");

    return EventResult::Success;
}

EventResult EGEClient::onFinish(int exitCode)
{
    (void) exitCode;
    disconnect();
    return EventResult::Success;
}

void EGEClient::onTick(TickCount tickCount)
{
    if(!isConnected())
        exit(tickCount);
}

SharedPtr<SFMLPacket> EGEClient::makePacket(sf::Packet& packet)
{
    return make<EGEPacket>(packet);
}

void EGEClient::disconnect()
{
    Client::disconnect();
    onDisconnect("Disconnected");

    /*if(m_clientTask)
    {
        m_clientTask->wait();

        // We don't need the task anymore.
        m_clientTask = SharedPtr<AsyncTask>();
    }*/
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
        if constexpr(EGECLIENT_DEBUG) std::cerr << "EGEClient: Sending requestObject from control";
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
        log(LogLevel::Warning) << "Client has no additional or default controller for object " << object->getObjectId();
        return;
    }

    auto controller = getController(object->getObjectId());
    DUMP(EGECLIENT_DEBUG, m_requestedObjects.count(object->getObjectId()));
    if(!controller && !m_requestedObjects.count(object->getObjectId()))
    {
        if constexpr(EGECLIENT_DEBUG) std::cerr << "EGEClient: Sending requestObject from requestControl";
        requestObject(object->getObjectId());
    }
    controller->sendRequest(data);

    // try to do it also on client...
    controller->handleRequest(data);
}

void EGEClient::requestObject(UidType id)
{
    if constexpr(EGECLIENT_DEBUG) std::cerr << "EGEClient: Requesting object " << id << " from server";
    DUMP(EGECLIENT_DEBUG, m_requestedObjects.count(id));
    m_requestedObjects.insert(id);
    DUMP(EGECLIENT_DEBUG, m_requestedObjects.count(id));
    send(EGEPacket::generateCSceneObjectRequest(id));
}

}
