#pragma once

#include "Client.h"

#include <map>
#include <SFML/Network.hpp>

namespace EGE
{

class Server
{
public:
    Server(int port = 0)
    : m_serverPort(port) {}

    void start();
    void close();

    // synchronous
    bool sendTo(const Packet& packet, int id);
    bool sendToAll(const Packet& packet);
    bool sendTo(const Packet& packet, std::function<bool(Client*)> predicate);

    std::weak_ptr<Client> getClient(int id);
    std::vector<std::weak_ptr<Client>> getClients(std::function<bool(Client*)> predicate);

    // removes client from array and disconnects it
    void kickClient(Client* client);

    // accepts new clients, removes disconnected clients, etc.
    void select();

    virtual EventResult onClientConnect(Client* client) {}
    virtual EventResult onClientDisconnect(Client* client) {}
    virtual EventResult onReceive(Client* client, const Packet& packet) {}

private:
    int addClient(std::shared_ptr<Client>);

    std::map<int, std::shared_ptr<Client>> m_clients;
    sf::TcpListener m_listener;
    sf::SocketSelector m_selector;
    int m_lastClientUid = 1;
    int m_serverPort = 0;
};

}
