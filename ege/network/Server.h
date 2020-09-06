/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "ClientConnection.h"
#include "Packet.h"

#include <ege/loop/EventResult.h>
#include <ege/main/Config.h>
#include <map>
#include <SFML/Network.hpp>

namespace EGE
{

class Server
{
public:
    Server(int port = 0)
    : m_serverPort(port) {}

    bool start();
    void close();

    // synchronous
    bool sendTo(std::shared_ptr<Packet> packet, int id);
    bool sendToAll(std::shared_ptr<Packet> packet);
    bool sendTo(std::shared_ptr<Packet> packet, std::function<bool(ClientConnection*)> predicate);

    std::weak_ptr<ClientConnection> getClient(int id);
    std::vector<std::weak_ptr<ClientConnection>> getClients(std::function<bool(ClientConnection*)> predicate);

    // removes client from array and disconnects it
    void kickClient(ClientConnection* client);

    // accepts new clients, removes disconnected clients, etc.
    void select();

    virtual EventResult onClientConnect(ClientConnection* client) { return EventResult::Failure; }
    virtual EventResult onClientDisconnect(ClientConnection* client) { return EventResult::Failure; }
    virtual EventResult onReceive(ClientConnection* client, std::shared_ptr<Packet> packet) { return EventResult::Failure; }

    virtual std::shared_ptr<ClientConnection> makeClient(Server* server, std::shared_ptr<sf::TcpSocket> socket) = 0;

private:
    int addClient(std::shared_ptr<ClientConnection>);

    std::map<int, std::shared_ptr<ClientConnection>> m_clients;
    sf::TcpListener m_listener;
    sf::SocketSelector m_selector;
    sf::Mutex m_clientsAccessMutex;
    int m_lastClientUid = 1;
    int m_serverPort = 0;
};

}
