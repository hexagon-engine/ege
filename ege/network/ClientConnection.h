/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Packet.h"
#include "NetworkEndpoint.h"

namespace EGE
{

class Server;

// abstract from NetworkEndpoint
class ClientConnection : public NetworkEndpoint
{
public:
    ClientConnection(Server* server, std::shared_ptr<sf::TcpSocket> socket)
    : m_server(server)
    {
        m_socket = socket;
    }

    void setID(int id)
    {
        m_id = id;
    }

    int getID()
    {
        return m_id;
    }

    // alias
    void kick()
    {
        disconnect();
    }

private:
    Server* m_server;
    int m_id = 0;
};

}
