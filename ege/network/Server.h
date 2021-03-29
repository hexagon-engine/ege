/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*/

#pragma once

#include "ClientConnection.h"
#include "Packet.h"

#include <ege/core/EventResult.h>
#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <functional>
#include <map>
#include <SFML/Network.hpp>

namespace EGE
{

class Server
{
public:
    typedef std::map<int, SharedPtr<ClientConnection>> ClientMap;

    Server(int port = 0)
    : m_serverPort(port) {}

    bool start();
    void close();

    // synchronous
    bool sendTo(SharedPtr<Packet> packet, int id);
    bool sendToAll(SharedPtr<Packet> packet);
    bool sendTo(SharedPtr<Packet> packet, std::function<bool(ClientConnection&)> predicate);

    std::weak_ptr<ClientConnection> getClient(int id);
    std::vector<std::weak_ptr<ClientConnection>> getClients(std::function<bool(ClientConnection&)> predicate);

    // removes client from array and disconnects it
    void kickClient(ClientConnection& client);

    // accepts new clients, removes disconnected clients, etc.
    void select();

    virtual EventResult onClientConnect(ClientConnection&) { return EventResult::Failure; }
    virtual EventResult onClientDisconnect(ClientConnection&) { return EventResult::Failure; }
    virtual EventResult onReceive(ClientConnection&, SharedPtr<Packet>) { return EventResult::Failure; }

    virtual SharedPtr<ClientConnection> makeClient(Server& server, SharedPtr<sf::TcpSocket> socket) = 0;

    ClientMap::iterator begin()
    {
        return m_clients.begin();
    }
    ClientMap::iterator end()
    {
        return m_clients.end();
    }

protected:
    sf::Mutex m_clientsAccessMutex;
    int m_serverPort = 0;

private:
    int addClient(SharedPtr<ClientConnection>);

    ClientMap m_clients;
    sf::TcpListener m_listener;
    sf::SocketSelector m_selector;
    int m_lastClientUid = 1;
};

}
