/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "EGEServer.h"

#include <ctime>
#include <ege/network/ClientConnection.h>
#include <ege/network/SFMLNetworkImpl.h>
#include <ege/util/Time.h>
#include <memory>

namespace EGE
{

class EGEClientConnection : public ClientConnection, public SFMLNetworkImpl
{
public:
    EGEClientConnection(EGEServer* server, std::shared_ptr<sf::TcpSocket> socket)
    : ClientConnection(server, socket)
    , m_lastRecv(server->time(Time::Unit::Seconds), Time::Unit::Seconds) {}

    virtual bool send(std::shared_ptr<EGE::Packet> packet)
    {
        return sendTo(this, packet);
    }

    virtual std::shared_ptr<EGE::Packet> receive()
    {
        return receiveFrom(this);
    }

    virtual std::shared_ptr<SFMLPacket> makePacket(sf::Packet& packet);

    void setLastRecvTime(Time t);

    Time getLastRecvTime()
    {
        return m_lastRecv;
    }

    bool wasPinged()
    {
        return m_pinged;
    }
    void setPinged(bool ping = true)
    {
        m_pinged = ping;
    }

    long long getControlledSceneObject()
    {
        return m_controlledSceneObjectId;
    }
    void setControlledSceneObject(long long id)
    {
        m_controlledSceneObjectId = id;
    }

private:
    long long m_controlledSceneObjectId = 0;
    Time m_lastRecv;
    bool m_pinged = false;
};

}
