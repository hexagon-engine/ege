/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Packet.h"

#include <memory>
#include <SFML/Network.hpp>

namespace EGE
{

class Server;

// abstract
class ClientConnection
{
public:
    ClientConnection(Server* server, std::shared_ptr<sf::TcpSocket> socket)
    : m_server(server), m_socket(socket)
    {}

    void setID(int id)
    {
        m_id = id;
    }

    int getID()
    {
        return m_id;
    }

    std::weak_ptr<sf::TcpSocket> getSocket()
    {
        return m_socket;
    }

    // synchronous
    bool send(std::shared_ptr<Packet> packet);
    std::shared_ptr<Packet> receive();
    void kick();

    bool isConnected();

    virtual std::shared_ptr<Packet> makePacket(const sf::Packet& packet) = 0;

private:
    Server* m_server;
    std::shared_ptr<sf::TcpSocket> m_socket;
    int m_id = 0;
    bool m_connected = true;
};

}
