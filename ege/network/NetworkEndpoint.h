/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <memory>
#include <SFML/Network.hpp>

#include "Packet.h"

namespace EGE
{

// abstract
class NetworkEndpoint
{
public:
    bool send(std::shared_ptr<Packet> packet);
    std::shared_ptr<Packet> receive();
    bool isConnected();

    std::weak_ptr<sf::TcpSocket> getSocket()
    {
        return m_socket;
    }

    // synchronous
    void disconnect();

    virtual std::shared_ptr<Packet> makePacket(const sf::Packet& packet) = 0;

protected:
    std::shared_ptr<sf::TcpSocket> m_socket;
    bool m_connected = true;
};

}
