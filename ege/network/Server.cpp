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

#include "Server.h"

#include <ege/debug/Logger.h>
#include <iostream>
#include <string.h>

namespace EGE
{

bool Server::start()
{
    sf::Socket::Status status = m_listener.listen(m_serverPort);
    if(status != sf::Socket::Done)
    {
        ege_log.error() << "Failed to start server on " << m_serverPort;
        return false;
    }
    m_selector.add(m_listener);
    ege_log.info() << "Server listening on " << m_serverPort;
    return true;
}

void Server::close()
{
    ege_log.info() << "Closing server";
    m_selector.clear();
    {
        sf::Lock lock(m_clientsAccessMutex);
        m_clients.clear();
    }
    m_listener.close();
}

// synchronous
bool Server::sendTo(SharedPtr<Packet> packet, int id)
{
    std::weak_ptr<ClientConnection> client = getClient(id);
    if(client.expired())
        return false;

    return client.lock()->send(packet);
}

bool Server::sendToAll(SharedPtr<Packet> packet)
{
    return sendTo(packet, [](ClientConnection&) { return true; });
}

bool Server::sendTo(SharedPtr<Packet> packet, std::function<bool(ClientConnection&)> predicate)
{
    bool success = true;
    sf::Lock lock(m_clientsAccessMutex);
    for(auto pr: m_clients)
    {
        if(predicate(*pr.second))
        {
            pr.second->send(packet);
        }
    }
    return success;
}

std::weak_ptr<ClientConnection> Server::getClient(int id)
{
    sf::Lock lock(m_clientsAccessMutex);
    return m_clients[id];
}

std::vector<std::weak_ptr<ClientConnection>> Server::getClients(std::function<bool(ClientConnection&)> predicate)
{
    std::vector<std::weak_ptr<ClientConnection>> clients;
    sf::Lock lock(m_clientsAccessMutex);
    for(auto pr: m_clients)
    {
        if(predicate(*pr.second))
            clients.push_back(pr.second);
    }
    return clients;
}

int Server::addClient(SharedPtr<ClientConnection> client)
{
    EventResult result = onClientConnect(*client);
    if(result == EventResult::Failure)
    {
        ege_log.error() << "Event ClientConnect failed (rejected by EventHandler)";
        return 0;
    }

    m_lastClientUid++;
    client->setID(m_lastClientUid);
    {
        sf::Lock lock(m_clientsAccessMutex);
        m_clients.insert(std::make_pair(m_lastClientUid, client));
    }
    m_selector.add(*client->getSocket().lock().get());
    return m_lastClientUid;
}

void Server::kickClient(ClientConnection& client)
{
    // Don't kick already kicked clients!
    if(client.getSocket().expired())
        return;

    ege_log.info() << "Kicking client (" << client.getSocket().lock()->getRemoteAddress() << ":" << client.getSocket().lock()->getRemotePort() << ")";
    onClientDisconnect(client);

    // close socket etc.
    m_selector.remove(*client.getSocket().lock().get());
    client.kick();

    // remove client from array
    sf::Lock lock(m_clientsAccessMutex);
    m_clients.erase(m_clients.find(client.getID()));
}

// accepts new clients, removes disconnected clients, etc.
void Server::select()
{
    strerror(errno);
    if(m_selector.wait(sf::seconds(2)))
    {
        if(m_selector.isReady(m_listener))
        {
            SharedPtr<sf::TcpSocket> socket = make<sf::TcpSocket>();
            sf::Socket::Status status2 = m_listener.accept(*socket);

            if(status2 == sf::Socket::Done)
            {
                SharedPtr<ClientConnection> client = makeClient(*this, socket);
                if(client)
                {
                    int id = addClient(client);
                    if(id)
                    {
                        // TODO: onClientSuccessfulConnect()
                        ege_log.info() << "Client connected (" << socket->getRemoteAddress() << ":" << socket->getRemotePort() << ")";
                    }
                }
            }
        }

        {
            sf::Lock lock(m_clientsAccessMutex);
            if(!m_clients.empty())
            {
                for(auto& pr : m_clients)
                {
                    std::weak_ptr<sf::TcpSocket> sck = pr.second->getSocket();
                    if(m_selector.isReady(*sck.lock().get()))
                    {
                        SharedPtr<Packet> packet = pr.second->receive();
                        if(packet)
                        {
                            EventResult result = onReceive(*pr.second, packet);
                            if(result == EventResult::Failure)
                            {
                                ege_log.error() << "Event Receive failed (rejected by EventHandler)";
                                kickClient(*pr.second);

                                // FIXME: update `it' instead of giving up on one client !! :)
                                break;
                            }
                        }
                        else
                        {
                            kickClient(*pr.second);

                            // FIXME: update `it' instead of giving up on one client !! :)
                            break;
                        }
                    }
                }
            }
        }

        // check if any client disconnected itself explicitly
        {
            sf::Lock lock(m_clientsAccessMutex);
            if(!m_clients.empty())
            {
                for(auto& pr : m_clients)
                {
                    if(!pr.second->isConnected())
                    {
                        ege_log.info() << "Kicking client " << pr.second->getSocket().lock()->getRemoteAddress() << ":" << pr.second->getSocket().lock()->getRemotePort() << " due to explicit disconnect";
                        kickClient(*pr.second);

                        // FIXME: update `it' instead of giving up on one client !! :)
                        break;
                    }
                }
            }
        }
    }
}

}
