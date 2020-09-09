/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

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

#define EGECLIENT_DEBUG 1

namespace EGE
{

class EGEClient : public Client, public SFMLNetworkImpl, public GameLoop, public EGEGame
{
public:
    EGEClient(sf::IpAddress addr, unsigned short port)
    : Client()
    , m_ip(addr)
    , m_port(port) {}

    virtual ~EGEClient();

    virtual std::shared_ptr<ObjectMap> getLoginData(std::shared_ptr<ObjectMap>) { return nullptr; }

    virtual EventResult onReceive(std::shared_ptr<Packet> packet);
    EventResult createSceneObjectFromData(std::shared_ptr<ObjectMap> object, long long id, std::string typeId);
    EventResult updateSceneObjectFromData(std::shared_ptr<ObjectMap> object, long long id);

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

    void setExitHandler(std::function<void(int)> func)
    {
        m_exitHandler = func;
    }

private:
    std::map<long long, EGEPacket::Type> m_uidMap;
    std::shared_ptr<AsyncTask> m_clientTask;
    std::function<void(int)> m_exitHandler;
    sf::IpAddress m_ip;
    unsigned short m_port;
};

}
