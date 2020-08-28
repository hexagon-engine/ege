#include "Server.h"

namespace EGE
{

bool Server::start()
{
    sf::Socket::Status status = m_listener.listen(m_serverPort);
    if(status != sf::Socket::Done)
    {
        std::cerr << "0011 EGE/network: Failed to start server on " << m_serverPort << std::cerr;
        return false;
    }
    std::cerr << "0010 EGE/network: Server listening on " << m_serverPort << std::cerr;
    return true;
}

void Server::close()
{
    std::cerr << "0012 EGE/network: Closing server" << std::cerr;
    m_listener.close();
}

// synchronous
bool Server::sendTo(const Packet& packet, int id)
{
    std::weak_ptr<Client> client = getClient(id);
    if(client.expired())
        return false;

    return client.lock()->send(packet);
}

bool Server::sendToAll(const Packet& packet)
{
    return sendTo(packet, [](Client*)->bool { return true; });
}

bool Server::sendTo(const Packet& packet, std::function<bool(Client*)> predicate)
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

std::weak_ptr<Client> Server::getClient(int id)
{
    return m_clients[id];
}

std::vector<std::weak_ptr<Client>> Server::getClients(std::function<bool(Client*)> predicate)
{
    std::vector<std::weak_ptr<Client>> clients;
    for(auto pr: m_clients)
    {
        if(predicate(pr.second.get()))
            clients.push_back(pr.second);
    }
    return clients;
}

int Server::addClient(std::shared_ptr<Client> client)
{
    EventResult result = onClientConnect(client.get());
    if(result == EventResult::Failure)
    {
        std::cerr << "0014 EGE/network: Event ClientConnect failed (rejected by EventHandler)" << std::endl;
        return 0;
    }

    m_lastClientUid++;
    m_clients.insert(std::make_pair(m_lastClientUid, client));
    return m_lastClientUid;
}

void Server::kickClient(Client* client)
{
    onClientDisconnect(client);

    // close socket etc.
    client->kick();

    // remove client from array
    m_clients.erase(m_clients.find(client->getID()));
}

// accepts new clients, removes disconnected clients, etc.
void Server::select()
{
    if(selector.wait(seconds(2)))
    {
        if(selector.isReady(listener))
        {
            std::shared_ptr<TcpSocket> socket = std::make_shared<TcpSocket>();
            Socket::Status status2 = listener.accept(*socket);

            if(status2 == Socket::Done)
            {
                std::shared_ptr<Client> client = std::make_shared<Client>(this, socket);
                if(client)
                {
                    int id = addClient(client);
                    if(id)
                    {
                        client->setID(id);
                        std::cerr << "0013 EGE/network: Client connected (ip=" << socket->getRemoteAddress() << ", port=" << socket->getRemotePort() << ")" << std::endl;
                    }
                }
            }
        }
        if(!clients.empty())
        {
            for(auto& pr : clients)
            {
                sf::TcpSocket* sck = pr.second->getSocket();
                if(selector.isReady(*sck))
                {
                    sf::Packet packet;
                    if(sck->receive(packet) == Socket::Done)
                    {
                        EventResult result = onReceive(pr.second.get(), Packet(packet));
                        if(result == EventResult::Failure)
                        {
                            std::cerr << "0014 EGE/network: Event Receive failed (rejected by EventHandler)" << std::endl;
                            kickClient(pr.second.get());
                        }
                    }
                    else
                    {
                        std::cerr << "0016 EGE/network: Socket Receive failed (system error)" << std::endl;
                        kickClient(pr.second.get());
                    }
                }
            }
        }
    }
}

}
