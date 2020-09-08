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

    virtual std::shared_ptr<ObjectMap> getLoginData(std::shared_ptr<ObjectMap>) { return nullptr; }

    virtual EventResult onReceive(std::shared_ptr<Packet> packet);

    virtual EventResult onLoad();
    virtual void onTick(long long tickCount);

    virtual void onExit(int exitCode);
    virtual EventResult onFinish(int exitCode) { return EventResult::Success; }

    virtual void onDisconnect(std::string reason) {}

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

private:
    std::map<long long, EGEPacket::Type> m_uidMap;
    sf::IpAddress m_ip;
    unsigned short m_port;
};

}
