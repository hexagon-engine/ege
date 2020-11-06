/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "ClientNetworkController.h"
#include "EGEGame.h"
#include "EGEPacket.h"

#include <ege/asyncLoop/ThreadSafeEventLoop.h>
#include <ege/gui/GameLoop.h>
#include <ege/network/Client.h>
#include <ege/network/Packet.h>
#include <ege/network/SFMLNetworkImpl.h>
#include <ege/network/SFMLPacket.h>
#include <ege/util/ObjectMap.h>
#include <memory>
#include <set>

#define EGECLIENT_DEBUG 1

#define EGE_SCENE_OBJECT_CREATOR(clazz) [](std::shared_ptr<EGE::Scene> scene) { return make<clazz>(scene); }

namespace EGE
{

class EGEClient : public Client, public SFMLNetworkImpl, public GameLoop, public EGEGame
{
public:
    EGEClient(sf::IpAddress addr, unsigned short port)
    : Client()
    , EGEGame()
    , m_ip(addr)
    , m_port(port) {}

    virtual ~EGEClient();

    virtual std::shared_ptr<ObjectMap> getLoginData(std::shared_ptr<ObjectMap>) { return nullptr; }
    virtual void onData(std::shared_ptr<ObjectMap>) {}

    virtual EventResult onReceive(std::shared_ptr<Packet> packet);
    EventResult createSceneObjectFromData(std::shared_ptr<ObjectMap> object, long long id, std::string typeId);
    EventResult updateSceneObjectFromData(std::shared_ptr<ObjectMap> object, long long id);

    virtual void setScene(std::shared_ptr<Scene> scene);

    virtual EventResult onLoad();
    virtual void onTick(long long tickCount);

    virtual void onExit(int) {}
    virtual EventResult onFinish(int exitCode);

    virtual void onDisconnect(std::string) {}

    virtual bool send(std::shared_ptr<Packet> packet)
    {
        return sendTo(this, packet);
    }

    bool sendWithUID(std::shared_ptr<EGEPacket> packet);

    virtual std::shared_ptr<Packet> receive()
    {
        return receiveFrom(this);
    }

    virtual std::shared_ptr<SFMLPacket> makePacket(sf::Packet& packet);
    virtual void disconnect();

    void setExitHandler(std::function<void(int)> func) { m_exitHandler = func; }

    std::shared_ptr<ClientNetworkController> getDefaultController() { return m_defaultController; }
    std::shared_ptr<ClientNetworkController> getController(long long objectId);
    std::shared_ptr<SceneObject> getDefaultControlledObject();
    std::shared_ptr<SceneObject> getControlledObject(long long objectId);

    virtual std::shared_ptr<ClientNetworkController> makeController(std::shared_ptr<SceneObject>) { return nullptr; }
    void control(std::shared_ptr<SceneObject> object, const ControlObject& data);
    void requestControl(std::shared_ptr<SceneObject> object, const ControlObject& data);

    void requestObject(long long id);

    void addSceneObjectCreator(std::string id, std::function<std::shared_ptr<SceneObject>(std::shared_ptr<Scene>)> creator)
    {
        m_sceneObjectCreators.push_back(std::make_pair(id, creator));
    }

private:
    std::map<long long, EGEPacket::Type> m_uidMap;
    std::shared_ptr<AsyncTask> m_clientTask;
    std::shared_ptr<ClientNetworkController> m_defaultController;
    std::map<long long, std::shared_ptr<ClientNetworkController>> m_controllersForObjects;
    std::function<void(int)> m_exitHandler;
    std::set<long long> m_requestedObjects;
    std::vector<std::pair<std::string, std::function<std::shared_ptr<SceneObject>(std::shared_ptr<Scene>)>>> m_sceneObjectCreators;
    sf::IpAddress m_ip;
    unsigned short m_port;
};

}
