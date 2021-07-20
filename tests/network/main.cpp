#include <testsuite/Tests.h>

#include <ege/network/TcpServer.h>
#include <ege/network/TcpClient.h>
#include <ege/network/TcpClientConnection.h>
#include <ege/network/SFMLPacket.h>

int PORT;

class MyPacket : public EGE::SFMLPacket
{
public:
    MyPacket() = default;

    explicit MyPacket(std::string string)
    : m_string(string) {}

    virtual bool deserializeFromSFML(sf::Packet& packet) override
    {
        if(!(packet >> m_string))
        {
            std::cerr << "fromSFMLPacket: expected std::string" << std::endl;
            return false;
        }
        return true;
    }

    virtual sf::Packet serializeToSFML() const override
    {
        sf::Packet packet;
        packet << m_string;
        return packet;
    }

    std::string getString() const { return m_string; }

private:
    std::string m_string;
};

class MyClientConnection;

class MyServer : public EGE::TcpServer<MyPacket, MyClientConnection>
{
public:
    virtual void onClientConnect(ClientConnection& client) override;
    virtual void onClientDisconnect(ClientConnection& client) override;
    virtual void onReceive(ClientConnection& client, Packet const& packet) override;

    bool running = true;
    int nextId = 0;
};

class MyClientConnection : public EGE::TcpClientConnection<MyServer>
{
public:
    MyClientConnection(MyServer& server)
    : EGE::TcpClientConnection<MyServer>(server) {}

    void setId(int id) { m_id = id;}
    int getId() const { return m_id; }

private:
    int m_id = -1;
};

void MyServer::onClientConnect(ClientConnection& client)
{
    client.setId(++nextId);
    client.send(MyPacket("Hello, " + client.getIPAddressString()));
    client.send(MyPacket("please login"));
    sendToAll(MyPacket(client.getIPAddressString() + " connected to server"));
}

void MyServer::onClientDisconnect(ClientConnection& client)
{
    sendToAll(MyPacket(client.getIPAddressString() + " disconnected from server"));
}

void MyServer::onReceive(ClientConnection& client, Packet const& packet)
{
    std::cerr << client.getIPAddressString() << " sent message: " << packet.getString() << std::endl;
}

class MyClient : public EGE::TcpClient<MyPacket>
{
public:
    virtual void onReceive(MyPacket const& packet) override
    {
        std::string msg = packet.getString();

        if(msg == "please login")
            send(MyPacket("yes, here you are: login 1234 password 1234."));

        std::cerr << "Server sent message: " << packet.getString() << std::endl;
    }
};

TESTCASE(abstract)
{
    srand(time(NULL));
    PORT = rand() % 64511 + 1024;

    MyServer server;
    MyClient client1;
    MyClient client2;
    MyClient client3;

    if(!server.listen(PORT))
    {
        std::cerr << "Failed to start server!" << std::endl;
        return 1;
    }

    std::cerr << "Server is running! :)" << std::endl;

    // Create 2 clients at once
    // wait a second to ensure that server is started
    sf::sleep(sf::seconds(1.f));
    if(!client1.connect(sf::IpAddress::LocalHost, PORT))
    {
        std::cerr << "Failed to connect to server!" << std::endl;
        return 1;
    }

    std::cerr << "Client 1 is running! :)" << std::endl;
    if(!client1.send(MyPacket("hello, Server! I'm Client ONE")))
    {
        std::cerr << "Failed to send packet!" << std::endl;
        return 1;
    }

    if(!client2.connect(sf::IpAddress::LocalHost, PORT))
    {
        std::cerr << "Failed to connect to server!" << std::endl;
        return 1;
    }

    std::cerr << "Client 2 is running! :)" << std::endl;
    if(!client2.send(MyPacket("hello, Server! I'm Client TWO")))
    {
        std::cerr << "Failed to send packet!" << std::endl;
        return 1;
    }

    // Create third client to test select errors
    // wait a second to ensure that server is started
    sf::sleep(sf::seconds(1.f));
    if(!client3.connect(sf::IpAddress::LocalHost, PORT))
    {
        std::cerr << "Failed to connect to server!" << std::endl;
        return 1;
    }

    std::cerr << "Client 2 is running! :)" << std::endl;
    if(!client3.send(MyPacket("hello, Server! I'm Client TWO")))
    {
        std::cerr << "Failed to send packet!" << std::endl;
        return 1;
    }

    // wait for enter
    getchar();

    ege_log.info() << "enter!";
    client1.disconnect();
    // wait for server reaction
    sf::sleep(sf::seconds(10.f));
    server.exit();
    // wait for client reaction
    sf::sleep(sf::seconds(1.f));
    client2.disconnect();
    client3.disconnect();
    server.running = false;

    return 0;
}

RUN_TESTS(network)
