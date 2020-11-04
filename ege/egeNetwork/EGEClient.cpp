/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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

bool EGEClient::sendWithUID(std::shared_ptr<EGEPacket> packet)
{
    // FIXME: some assertion?
    long long uid = packet->getArgs()->getObject("uid").lock()->asInt();
    m_uidMap[uid] = packet->getType();
    return send(packet);
}

EventResult EGEClient::onReceive(std::shared_ptr<Packet> packet)
{
    EGEPacket* egePacket = (EGEPacket*)packet.get();

    /*if constexpr(EGECLIENT_DEBUG)
    {
        sf::Packet sfPacket = egePacket->toSFMLPacket();
        std::cerr << "Client: Hex dump: " << std::endl;
        hexDump(sfPacket.getData(), sfPacket.getDataSize(), HexDumpSettings{8});
    }*/

    /*if constexpr(EGECLIENT_DEBUG)
    {
        std::cerr << "Client: EGEPacket(" << EGEPacket::typeString(egePacket->getType()) << ") ";
        std::cerr << egePacket->getArgs()->toString() << std::endl;
    }*/

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
            int value = egePacket->getArgs()->getObject("value").lock()->asInt();
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
            std::shared_ptr<ObjectMap> args = egePacket->getArgs();
            sendWithUID(EGEPacket::generateCLogin(getLoginData(args)));
        }
        break;
    case EGEPacket::Type::SDisconnectReason:
        {
            std::shared_ptr<ObjectMap> args = egePacket->getArgs();
            auto obj = args->getObject("message");
            ASSERT(!obj.expired());
            ASSERT(obj.lock()->isString());
            onDisconnect(obj.lock()->asString());
            disconnect();
        }
        break;
    case EGEPacket::Type::SSceneObjectCreation:
        {
            std::shared_ptr<ObjectMap> args = egePacket->getArgs();
            auto object = args->getObject("object");
            auto id = args->getObject("id");
            auto typeId = args->getObject("typeId");
            ASSERT(!object.expired() && object.lock()->isMap());
            ASSERT(!id.expired() && id.lock()->isInt());
            ASSERT(!typeId.expired() && typeId.lock()->isString());
            //std::cerr << "SSceneObjectCreation " << id.lock()->asInt() << std::endl;
            return createSceneObjectFromData(std::dynamic_pointer_cast<ObjectMap>(object.lock()), id.lock()->asInt(), typeId.lock()->asString());
        }
        break;
    case EGEPacket::Type::SSceneObjectUpdate:
        {
            std::shared_ptr<ObjectMap> args = egePacket->getArgs();
            auto object = args->getObject("object");
            auto id = args->getObject("id");
            ASSERT(!object.expired() && object.lock()->isMap());
            ASSERT(!id.expired() && id.lock()->isInt());
            return updateSceneObjectFromData(std::dynamic_pointer_cast<ObjectMap>(object.lock()), id.lock()->asInt());
        }
        break;
    case EGEPacket::Type::SSceneObjectDeletion:
        {
            std::shared_ptr<ObjectMap> args = egePacket->getArgs();
            auto id = args->getObject("id");
            ASSERT(!id.expired() && id.lock()->isInt());
            auto scene = getScene();
            if(!scene) //scene not created
                return EventResult::Success;
            auto sceneObject = scene->getObject(id.lock()->asInt());
            if(!sceneObject) // Yay! We have predicted that the object will be removed! [or bugged server :)]
                return EventResult::Success;
            //std::cerr << "SSceneObjectDeletion " << id.lock()->asInt() << std::endl;
            sceneObject->setDead();
        }
        break;
    case EGEPacket::Type::SDefaultControllerId:
        {
            std::shared_ptr<ObjectMap> args = egePacket->getArgs();
            auto id = args->getObject("id");
            ASSERT(!id.expired() && id.lock()->isInt());
            auto scene = getScene();
            if(!scene) //scene not created
                return EventResult::Success;
            long long _id = id.lock()->asInt();
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
            std::shared_ptr<ObjectMap> args = egePacket->getArgs();
            auto id = args->getObject("id");
            ASSERT(!id.expired() && id.lock()->isInt());
            auto data = args->getObject("data");
            ASSERT(!data.expired() && data.lock()->isMap());
            auto data_map = std::dynamic_pointer_cast<ObjectMap>(data.lock());
            auto data_name = data_map->getObject("type");
            ASSERT(!data_name.expired() && data_name.lock()->isString());
            auto data_args = data_map->getObject("args");
            ASSERT(!data_args.expired() && data_args.lock()->isMap());

            if(!getScene()) // cannot control object when no scene is created!
                return EventResult::Failure;

            auto controller = getController(id.lock()->asInt());
            if(controller)
                controller->handleRequest(ControlObject(data_name.lock()->asString(), std::dynamic_pointer_cast<ObjectMap>(data_args.lock())));
        }
        break;
    case EGEPacket::Type::_Version:
        {
            auto value = egePacket->getArgs()->getObject("value");
            ASSERT(!value.expired() && value.lock()->isUnsignedInt());
            auto str = egePacket->getArgs()->getObject("string");
            ASSERT(!str.expired() && str.lock()->isString());

            int _value = value.lock()->asInt();
            std::string _str = str.lock()->asString();

            if(_value != getVersion())
            {
                err() << "Invalid server version! (need " << getVersion() << ", got " << _value << ")";
                return EventResult::Failure;
            }

            if(_str != getVersionString())
            {
                err() << "Invalid server! (need '" << getVersionString() << "', got '" << _str << "')";
                return EventResult::Failure;
            }

            // TODO: Force it on server-side and kick player when it doesn't send
            // its _Version packet
            send(EGEPacket::generate_Version(getVersion(), getVersionString()));
        }
        break;
    default:
        err() << "0022 EGE/egeNetwork: Unimplemented packet handler: " + EGEPacket::typeString(egePacket->getType());
        return EventResult::Failure;
    }

    return EventResult::Success;
}

EventResult EGEClient::createSceneObjectFromData(std::shared_ptr<ObjectMap> object, long long id, std::string typeId)
{
    if(!getScene()) //scene not created
        return EventResult::Success;

    EGEGame::GPOM* gpom = getGameplayObjectManager().get();
    ASSERT(gpom);

    auto func = gpom->sceneObjectCreators.findById(typeId);
    if(!func) //game version mismatch?
    {
        err() << "Not found '" << typeId << "' in GPOM! Did you forget to add SceneObjectCreator?";
        return EventResult::Failure;
    }

    // Call `func' that was registered by user.
    std::shared_ptr<SceneObject> sceneObject = (*func)(getScene());
    sceneObject->setObjectId(id); // Don't assign ID automatically!
    sceneObject->deserialize(object);
    //m_requestedObjects.erase(id);
    getScene()->addObject(sceneObject);

    return EventResult::Success;
}

EventResult EGEClient::updateSceneObjectFromData(std::shared_ptr<ObjectMap> object, long long id)
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

void EGEClient::setScene(std::shared_ptr<Scene> scene)
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
                                });
    scene->setRemoveObjectCallback([this](std::shared_ptr<SceneObject> object) {
                                   // Remove controller from controller map.
                                   auto it = m_controllersForObjects.find(object->getObjectId());
                                   if(it != m_controllersForObjects.end())
                                        m_controllersForObjects.erase(it);
                                });
    EGEGame::setScene(scene);
}

EventResult EGEClient::onLoad()
{
    if(!EGEGame::initialize())
        return EventResult::Failure;

    // Add all SceneObjectCreators
    {
        auto gpom = getGameplayObjectManager();
        for(auto pr: m_sceneObjectCreators)
        {
            gpom->sceneObjectCreators.add(pr.first, new std::function(
                                        [pr](std::shared_ptr<EGE::Scene> scene)
                                        {
                                            return pr.second(scene);
                                        }
                                    ));
        }
    }

    // Run client thread
    auto clientNetworkWorker = [this]()->int {
        log() << "001E EGE/egeNetwork: Starting client";
        if(!connect(m_ip, m_port))
            return 1;

        send(EGEPacket::generate_ProtocolVersion(EGE_PROTOCOL_VERSION));

        while(isRunning())
        {
            update();
            if(!isConnected())
                m_running = false;
        }

        return 0;
    };
    auto clientNetworkCallback = [this](AsyncTask::State state) {
        log() << "001F EGE/egeNetwork: Closing client";

        exit(state.returnCode);

        if(m_exitHandler)
            m_exitHandler(state.returnCode);
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

void EGEClient::onTick(long long tickCount)
{
    if(!isConnected())
        exit(tickCount);
}

std::shared_ptr<SFMLPacket> EGEClient::makePacket(sf::Packet& packet)
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
        m_clientTask = std::shared_ptr<AsyncTask>();
    }*/
}

std::shared_ptr<ClientNetworkController> EGEClient::getController(long long objectId)
{
    return m_controllersForObjects[objectId];
}

void EGEClient::control(std::shared_ptr<SceneObject> object, const ControlObject& data)
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

void EGEClient::requestControl(std::shared_ptr<SceneObject> object, const ControlObject& data)
{
    if(!object)
    {
        if(m_defaultController)
            m_defaultController->sendRequest(data);

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

void EGEClient::requestObject(long long id)
{
    if constexpr(EGECLIENT_DEBUG) std::cerr << "EGEClient: Requesting object " << id << " from server";
    DUMP(EGECLIENT_DEBUG, m_requestedObjects.count(id));
    m_requestedObjects.insert(id);
    DUMP(EGECLIENT_DEBUG, m_requestedObjects.count(id));
    send(EGEPacket::generateCSceneObjectRequest(id));
}

}
