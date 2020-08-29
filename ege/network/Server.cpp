/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Server.h"

#include <iostream>

namespace EGE
{

bool Server::start()
{
    sf::Socket::Status status = m_listener.listen(m_serverPort);
    if(status != sf::Socket::Done)
    {
        std::cerr << "0011 EGE/network: Failed to start server on " << m_serverPort << std::endl;
        return false;
    }
    std::cerr << "0010 EGE/network: Server listening on " << m_serverPort << std::endl;
    return true;
}

void Server::close()
{
    std::cerr << "0012 EGE/network: Closing server" << std::endl;
    m_listener.close();
}

// synchronous
bool Server::sendTo(std::shared_ptr<Packet> packet, int id)
{
    std::weak_ptr<ClientConnection> client = getClient(id);
    if(client.expired())
        return false;

    return client.lock()->send(packet);
}

bool Server::sendToAll(std::shared_ptr<Packet> packet)
{
    return sendTo(packet, [](ClientConnection*)->bool { return true; });
}

bool Server::sendTo(std::shared_ptr<Packet> packet, std::function<bool(ClientConnection*)> predicate)
{
    bool success = true;
    for(auto pr: m_clients)
    {
        if(predicate(pr.second.get()))
        {
            pr.second->send(packet);
        }
    }
    return success;
}

std::weak_ptr<ClientConnection> Server::getClient(int id)
{
    return m_clients[id];
}

std::vector<std::weak_ptr<ClientConnection>> Server::getClients(std::function<bool(ClientConnection*)> predicate)
{
    std::vector<std::weak_ptr<ClientConnection>> clients;
    for(auto pr: m_clients)
    {
        if(predicate(pr.second.get()))
            clients.push_back(pr.second);
    }
    return clients;
}

int Server::addClient(std::shared_ptr<ClientConnection> client)
{
    EventResult result = onClientConnect(client.get());
    if(result == EventResult::Failure)
    {
        std::cerr << "0014 EGE/network: Event ClientConnect failed (rejected by EventHandler)" << std::endl;
        return 0;
    }

    m_lastClientUid++;
    client->setID(m_lastClientUid);
    m_clients.insert(std::make_pair(m_lastClientUid, client));
    return m_lastClientUid;
}

void Server::kickClient(ClientConnection* client)
{
    onClientDisconnect(client);

    // close socket etc.
    client->kick();
    m_selector.remove(*client->getSocket().lock().get());

    // remove client from array
    m_clients.erase(m_clients.find(client->getID()));
}

// accepts new clients, removes disconnected clients, etc.
void Server::select()
{
    // check if any client disconnected itself explicitly
    if(!m_clients.empty())
    {
        for(auto& pr : m_clients)
        {
            if(!pr.second->isConnected())
            {
                std::cerr << "0018 EGE/network: Kicking client " << pr.second->getSocket().lock()->getRemoteAddress() << ":" << pr.second->getSocket().lock()->getRemotePort() << " due to explicit disconnect" << std::endl;
                kickClient(pr.second.get());
            }
        }
    }

    if(m_selector.wait(sf::seconds(2)))
    {
        if(m_selector.isReady(m_listener))
        {
            std::shared_ptr<sf::TcpSocket> socket = std::make_shared<sf::TcpSocket>();
            sf::Socket::Status status2 = m_listener.accept(*socket);

            if(status2 == sf::Socket::Done)
            {
                std::shared_ptr<ClientConnection> client = makeClient(this, socket);
                if(client)
                {
                    int id = addClient(client);
                    if(id)
                    {
                        // TODO: onm_clientsuccessfulConnect()
                        std::cerr << "0013 EGE/network: Client connected (" << socket->getRemoteAddress() << ":" << socket->getRemotePort() << ")" << std::endl;
                    }
                }
            }
        }
        if(!m_clients.empty())
        {
            for(auto& pr : m_clients)
            {
                std::weak_ptr<sf::TcpSocket> sck = pr.second->getSocket();
                if(m_selector.isReady(*sck.lock().get()))
                {
                    std::shared_ptr<Packet> packet = pr.second->receive();
                    if(packet)
                    {
                        EventResult result = onReceive(pr.second.get(), packet);
                        if(result == EventResult::Failure)
                        {
                            std::cerr << "0014 EGE/network: Event Receive failed (rejected by EventHandler)" << std::endl;
                            kickClient(pr.second.get());
                        }
                    }
                    else
                    {
                        kickClient(pr.second.get());
                    }
                }
            }
        }
    }
}

}
