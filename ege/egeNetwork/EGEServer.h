/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "EGEGame.h"
#include "ServerNetworkController.h"

#include <ege/asyncLoop/ThreadSafeEventLoop.h>
#include <ege/gui/GameLoop.h>
#include <ege/network/Packet.h>
#include <ege/network/Server.h>
#include <ege/util/ObjectMap.h>
#include <memory>

#define EGESERVER_DEBUG 1
#define PING_DEBUG 0

namespace EGE
{

class EGEClientConnection;

class EGEServer : public Server, public GameLoop, public EGEGame
{
public:
    EGEServer(int port)
    : Server(port) {}

    virtual EventResult onClientConnect(ClientConnection* client);
    virtual EventResult onClientDisconnect(ClientConnection* client);
    virtual EventResult onReceive(ClientConnection* client, std::shared_ptr<Packet> packet);

    // to be overridden by user
    // args: client, reason message
    virtual void onClientDisconnect(ClientConnection*, std::string) {}

    virtual std::shared_ptr<ObjectMap> getLoginData(ClientConnection*) { return nullptr; }

    // Accept all logins by default, without doing anything with userData!
    // Arguments:                     client                userData
    virtual EventResult onLogin(EGEClientConnection*, std::shared_ptr<ObjectMap>);

    virtual EventResult onLoad();
    virtual void onTick(long long tickCount);

    virtual void onExit(int exitCode);
    virtual EventResult onFinish(int) { return EventResult::Success; }

    void kickClientWithReason(EGEClientConnection* client, std::string reason);
    virtual std::shared_ptr<ClientConnection> makeClient(Server* server, std::shared_ptr<sf::TcpSocket> socket);
    virtual void setScene(std::shared_ptr<Scene> scene);
    std::shared_ptr<ServerNetworkController> getController(long long objectId);

    void setDefaultController(EGEClientConnection* client, std::shared_ptr<SceneObject> sceneObject);

    virtual std::shared_ptr<ServerNetworkController> makeController(std::shared_ptr<SceneObject>) { return nullptr; }
    void control(std::shared_ptr<SceneObject> object, const ControlObject& data);
    void requestControl(std::shared_ptr<SceneObject> object, const ControlObject& data);

private:
    std::map<long long, std::shared_ptr<ServerNetworkController>> m_controllersForObjects;
};

}
