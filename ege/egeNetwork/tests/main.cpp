#include <testsuite/Tests.h>
#include <ege/egeNetwork/EGEClient.h>
#include <ege/egeNetwork/EGEPacket.h>
#include <ege/egeNetwork/EGEServer.h>
#include <ege/util/ObjectInt.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/ObjectString.h>
#include <iomanip>
#include <iostream>

struct HexDumpSettings
{
    int width;
};

void hexDump(const void* data, size_t size, HexDumpSettings settings)
{
    for(size_t s = 0; s < size / settings.width + 1; s++)
    {
        std::cerr << std::hex << std::setfill('0') << std::setw(8) << s * settings.width << "   ";

        // data as HEX DUMP
        for(size_t t = 0; t < settings.width; t++)
        {
            size_t off = s * settings.width + t;
            if(off < size)
            {
                unsigned char _data = ((unsigned char*)data)[off] & 0xFF;
                std::cerr << std::hex << std::setfill('0') << std::setw(2) << (int)_data << " ";
            }
            else
            {
                std::cerr << "   ";
            }
        }

        std::cerr << "  ";

        // and as CHARACTERS
        for(size_t t = 0; t < settings.width; t++)
        {
            size_t off = s * settings.width + t;
            if(off < size)
            {
                unsigned char _data = ((unsigned char*)data)[off] & 0xFF;
                if(_data < 32 || _data >= 127)
                    _data = '.';
                std::cerr << _data << " ";
            }
        }
        std::cout << std::endl;
    }
}

TESTCASE(converter)
{
    std::shared_ptr<EGE::ObjectMap> map = std::make_shared<EGE::ObjectMap>();
    map->addObject("packetTestInt", std::make_shared<EGE::ObjectInt>(62452));
    map->addObject("String", std::make_shared<EGE::ObjectString>("test"));
    map->addObject("IntAsString", std::make_shared<EGE::ObjectString>("543"));
    std::shared_ptr<EGE::ObjectMap> subMap = std::make_shared<EGE::ObjectMap>();
    subMap->addObject("MapTest", std::make_shared<EGE::ObjectString>("YYYTEST555??"));
    map->addObject("SubMapTest", subMap);

    std::cerr << "Object: " << map->toString() << std::endl;

    EGE::EGEPacket packet(EGE::EGEPacket::Type::_Data, map);
    sf::Packet sfPacket = packet.toSFMLPacket();

    std::cerr << "Hex dump: " << std::endl;
    hexDump(sfPacket.getData(), sfPacket.getDataSize(), HexDumpSettings{8});

    EGE::EGEPacket packet2(sfPacket);
    std::cerr << "Parsed object: " << packet2.getArgs()->toString() << std::endl;
    std::cerr << std::dec;
}

TESTCASE(server)
{
    srand(time(nullptr));
    int PORT = rand() % 63536 + 2000;

    auto serverThread = [PORT]() {
        EGE::EGEServer server(PORT);
        return server.run();
    };
    sf::Thread thread1(serverThread);
    thread1.launch();
}

TESTCASE(client)
{
    int PORT = 0;

    // yes, there is no error handling...
    std::cout << "Enter port: ";
    std::cin >> PORT;

    auto clientThread = [PORT]() {
        EGE::EGEClient client(sf::IpAddress::LocalHost, PORT);
        return client.run();
    };
    sf::Thread thread2(clientThread);
    thread2.launch();
}

RUN_TESTS(egeNetwork)
