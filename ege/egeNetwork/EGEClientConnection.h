/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ctime>
#include <ege/network/ClientConnection.h>
#include <ege/network/SFMLNetworkImpl.h>
#include <memory>

namespace EGE
{

class EGEClientConnection : ClientConnection, SFMLNetworkImpl
{
public:
    EGEClientConnection(Server* server, std::shared_ptr<sf::TcpSocket> socket)
    : ClientConnection(server, socket)
    , m_lastRecv(::time(nullptr)) {}

    virtual bool send(std::shared_ptr<EGE::Packet> packet)
    {
        return sendTo(this, packet);
    }

    virtual std::shared_ptr<EGE::Packet> receive()
    {
        return receiveFrom(this);
    }

    virtual std::shared_ptr<SFMLPacket> makePacket(sf::Packet& packet);

    void setLastRecvTime(time_t t);

private:
    time_t m_lastRecv;
};

}
