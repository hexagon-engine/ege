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
    // almost always synchronous
    virtual bool send(std::shared_ptr<Packet> packet) = 0;
    virtual std::shared_ptr<Packet> receive() = 0;

    bool isConnected()
    {
        return m_connected;
    }

    std::weak_ptr<sf::TcpSocket> getSocket()
    {
        return m_socket;
    }

    sf::Mutex& getMutex()
    {
        return m_accessMutex;
    }

    // synchronous
    void disconnect();

protected:
    std::shared_ptr<sf::TcpSocket> m_socket;
    bool m_connected = true;
    sf::Mutex m_accessMutex;
};

}
