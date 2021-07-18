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

#include "TcpClientConnection.h"
#include "TcpEndpoint.h"

#include <ege/core/MainLoop.h>
#include <ege/debug/Logger.h>
#include <ege/util/PointerUtils.h>
#include <map>
#include <SFML/Network.hpp>
#include <string.h>

namespace EGE
{

// P must be derived from TcpPacket
// C must be derived from TcpClientConnection
template<class P, class C>
class TcpServer : public MainLoop
{
public:
    using Packet = P;
    using ClientConnection = C;
    using ClientList = std::list<std::shared_ptr<ClientConnection>>;

    bool listen(Uint16 port, sf::IpAddress address = sf::IpAddress::Any)
    {
        if(m_listener.listen(port, address) != sf::Socket::Done)
        {
            ege_log.error() << "TcpServer: Failed to listen on " << address.toString() << ":" << port;
            return false;
        }
        m_selector.add(m_listener);
        ege_log.error() << "TcpServer: Listening on " << address.toString() << ":" << port;

        addAsyncTask(make<AsyncTask>([this](AsyncTask& task) {
            while(true)
            {
                update();
                if(task.stopRequested())
                    return 0;
            }
        }, [this](AsyncTask::State state) { ege_log.info() << "TcpServer: Stopped!"; exit(0); }), "TcpServer");
        return true;
    }

    void update()
    {
        if(m_selector.wait(sf::seconds(0.5)))
        {
            // Server
            if(m_selector.isReady(m_listener))
            {
                auto connection = make<ClientConnection>(static_cast<typename ClientConnection::Server&>(*this));
                if(m_listener.accept(connection->socket()) != sf::Socket::Done)
                    ege_log.error() << "TcpServer: Error accepting client";
                else
                {
                    onClientConnect(*connection);
                    ege_log.info() << "TcpServer: Client connected: " << connection->toString();
                    {
                        std::lock_guard<std::mutex> lock(m_clientsMutex);
                        m_clients.push_back(connection);
                    }
                    m_selector.add(connection->socket());
                }
            }
            // Clients
            std::lock_guard<std::mutex> lock(m_clientsMutex);
            for(auto& it: m_clients)
            {
                if(m_selector.isReady(it->socket()))
                {
                    auto packet = it->receive();
                    if(!packet.isValid())
                    {
                        m_selector.remove(it->socket());
                        it->disconnect();
                    }
                    else
                        onReceive(*it, packet);
                }
            }
        }

        // Clean up disconnected sockets
        std::lock_guard<std::mutex> lock(m_clientsMutex);
        for(auto it = m_clients.begin(); it != m_clients.end();)
        {
            auto oldIt = it++;
            auto& client = *oldIt;
            if(!client->isConnected())
            {
                ege_log.info() << "TcpServer: Client disconnected: " << client->toString();
                onClientDisconnect(*client);
                m_clients.erase(oldIt);
            }
        }
    }

    virtual void onReceive(ClientConnection& client, const Packet& packet) = 0;
    virtual void onClientConnect(ClientConnection& client) = 0;
    virtual void onClientDisconnect(ClientConnection& client) = 0;

    template<class Predicate>
    void sendToIf(Packet const& packet, Predicate predicate)
    {
        std::lock_guard<std::mutex> lock(m_clientsMutex);
        for(auto& client: m_clients)
        {
            if(predicate(*client))
                client->send(packet);
        }
    }

    void sendToAll(Packet const& packet)
    {
        std::lock_guard<std::mutex> lock(m_clientsMutex);
        for(auto& client: m_clients)
        {
            client->send(packet);
        }
    }

    template<class Predicate>
    void disconnectIf(Predicate predicate)
    {
        std::lock_guard<std::mutex> lock(m_clientsMutex);
        for(auto& client: m_clients)
        {
            if(predicate(*client))
                client->disconnect();
        }
    }

    void disconnect(sf::TcpSocket& socket)
    {
        m_selector.remove(socket);
        socket.disconnect();
    }

    ClientList& clients() { return m_clients; }
    const ClientList& clients() const { return m_clients; }

private:
    ClientList m_clients;
    std::mutex m_clientsMutex;

    sf::TcpListener m_listener;
    sf::SocketSelector m_selector;
};

}
