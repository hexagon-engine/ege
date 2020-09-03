/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "EGEClientConnection.h"

#include <ege/asyncLoop/ThreadSafeEventLoop.h>
#include <ege/gui/GameLoop.h>
#include <ege/network/ClientConnection.h>
#include <ege/network/Packet.h>
#include <ege/network/Server.h>
#include <ege/util/ObjectMap.h>
#include <memory>

#define EGESERVER_DEBUG 1

namespace EGE
{

class EGEServer : public Server, public GameLoop
{
public:
    EGEServer(int port)
    : Server(port) {}

    virtual EventResult onClientConnect(ClientConnection* client);
    virtual EventResult onClientDisconnect(ClientConnection* client);
    virtual EventResult onReceive(ClientConnection* client, std::shared_ptr<Packet> packet);

    virtual std::shared_ptr<ObjectMap> getLoginData(ClientConnection* client) { return nullptr; }

    // Accept all logins by default, without doing anything with userData!
    // Arguments:                     client                userData
    virtual EventResult onLogin(EGEClientConnection*, std::shared_ptr<ObjectMap>) { return EGE::EventResult::Success; };

    virtual EventResult onLoad();
    virtual void onTick(long long tickCount);

    virtual void onExit(int exitCode);
    virtual EventResult onFinish(int exitCode) { return EventResult::Success; }

    virtual std::shared_ptr<ClientConnection> makeClient(Server* server, std::shared_ptr<sf::TcpSocket> socket);
};

}
