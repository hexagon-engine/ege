#include <testsuite/Tests.h>

#include <ege/network/Server.h>
#include <ege/network/Client.h>
#include <ege/network/SFMLNetworkImpl.h>

int PORT;

class MyPacket : public EGE::SFMLPacket
{
public:
    // sender-side
    MyPacket(std::string str)
    : m_string(str) {}

    // receiver-side
    MyPacket(sf::Packet& packet)
    : m_string("<invalid packet>")
    {
        if(!fromSFMLPacket(packet))
            std::cerr << "invalid packet received!" << std::endl;
    }

    virtual bool fromSFMLPacket(sf::Packet& packet)
    {
        if(!(packet >> m_string))
        {
            std::cerr << "fromSFMLPacket: expected std::string" << std::endl;
            return false;
        }
        m_valid = true;
        return true;
    }

    virtual sf::Packet toSFMLPacket()
    {
        sf::Packet packet;
        packet << m_string;
        return packet;
    }

    std::string getString()
    {
        return m_string;
    }

    bool isValid()
    {
        return m_valid;
    }

private:
    std::string m_string;
    bool m_valid = false;
};

class MyClientConnection : public EGE::ClientConnection, public EGE::SFMLNetworkImpl
{
public:
    MyClientConnection(EGE::Server* server, std::shared_ptr<sf::TcpSocket> socket)
    : EGE::ClientConnection(server, socket) {}

    std::shared_ptr<EGE::SFMLPacket> makePacket(sf::Packet& packet)
    {
        return make<MyPacket>(packet);
    }

    virtual bool send(std::shared_ptr<EGE::Packet> packet)
    {
        return sendTo(this, packet);
    }

    virtual std::shared_ptr<EGE::Packet> receive()
    {
        return receiveFrom(this);
    }
};

class MyServer : public EGE::Server
{
public:
    MyServer()
    : EGE::Server(PORT) {}

    std::shared_ptr<EGE::ClientConnection> makeClient(EGE::Server* server, std::shared_ptr<sf::TcpSocket> socket)
    {
        return make<MyClientConnection>(server, socket);
    }

    virtual EGE::EventResult onClientConnect(EGE::ClientConnection* client)
    {
        if(client->getSocket().expired())
            return EGE::EventResult::Failure;

        client->send(make<MyPacket>("Hello, " + client->getSocket().lock()->getRemoteAddress().toString()));
        client->send(make<MyPacket>("please login"));
        sendToAll(make<MyPacket>(client->getSocket().lock()->getRemoteAddress().toString() + " connected to server"));
        return EGE::EventResult::Success;
    }

    virtual EGE::EventResult onClientDisconnect(EGE::ClientConnection* client)
    {
        if(client->getSocket().expired())
            return EGE::EventResult::Failure;

        sendToAll(make<MyPacket>(client->getSocket().lock()->getRemoteAddress().toString() + " disconnected from server"));
        return EGE::EventResult::Success;
    }

    virtual EGE::EventResult onReceive(EGE::ClientConnection* client, std::shared_ptr<EGE::Packet> packet)
    {
        MyPacket* mypacket = (MyPacket*) packet.get();
        std::cerr << client->getID() << " sent message: " << mypacket->getString() << std::endl;
        return EGE::EventResult::Success;
    }

    bool running = true;
};

class MyClient : public EGE::Client, public EGE::SFMLNetworkImpl
{
public:
    std::shared_ptr<EGE::SFMLPacket> makePacket(sf::Packet& packet)
    {
        return make<MyPacket>(packet);
    }

    virtual EGE::EventResult onReceive(std::shared_ptr<EGE::Packet> packet)
    {
        MyPacket* mypacket = (MyPacket*) packet.get();
        std::string msg = mypacket->getString();

        if(msg == "please login")
            send(make<MyPacket>("yes, here you are: login 1234 password 1234."));

        std::cerr << "Server sent message: " << mypacket->getString() << std::endl;
        return EGE::EventResult::Success;
    }

    virtual bool send(std::shared_ptr<EGE::Packet> packet)
    {
        return sendTo(this, packet);
    }

    virtual std::shared_ptr<EGE::Packet> receive()
    {
        return receiveFrom(this);
    }
};

TESTCASE(abstract)
{
    srand(time(NULL));
    PORT = rand() % 64511 + 1024;

    MyServer server;
    MyClient client1;
    MyClient client2;

    auto serverThreadFunc = [&server]() {

        if(!server.start())
        {
            std::cerr << "Failed to start server!" << std::endl;
            tests.fail();
            return;
        }

        std::cerr << "Server is running! :)" << std::endl;
        while(server.running)
            server.select();
        std::cerr << "Server finished! :)" << std::endl;

    };
    sf::Thread serverThread(serverThreadFunc);
    serverThread.launch();


    auto client1ThreadFunc = [&client1]() {

        // wait a second to ensure that server is started
        sf::sleep(sf::seconds(1.f));
        if(!client1.connect(sf::IpAddress::LocalHost, PORT))
        {
            std::cerr << "Failed to connect to server!" << std::endl;
            tests.fail();
            return;
        }

        std::cerr << "Client 1 is running! :)" << std::endl;
        client1.send(make<MyPacket>("hello, Server! I'm Client ONE"));
        while(client1.isConnected())
            client1.update();
        std::cerr << "Client 1 finished! :)" << std::endl;

    };
    sf::Thread client1Thread(client1ThreadFunc);
    client1Thread.launch();


    auto client2ThreadFunc = [&client2]() {

        // wait 2 seconds to ensure that server is started
        sf::sleep(sf::seconds(2.f));
        if(!client2.connect(sf::IpAddress::LocalHost, PORT))
        {
            std::cerr << "Failed to connect to server!" << std::endl;
            tests.fail();
            return;
        }

        std::cerr << "Client 2 is running! :)" << std::endl;
        client2.send(make<MyPacket>("hello, Server! I'm Client TWO"));
        while(client2.isConnected())
            client2.update();
        std::cerr << "Client 2 finished! :)" << std::endl;
    };
    sf::Thread client2Thread(client2ThreadFunc);
    client2Thread.launch();

    // wait for enter
    getchar();

    client1.disconnect();
    // wait for server reaction
    sf::sleep(sf::seconds(10.f));
    server.close();
    // wait for client reaction
    sf::sleep(sf::seconds(1.f));
    client2.disconnect();
    server.running = false;
}

RUN_TESTS(network)
