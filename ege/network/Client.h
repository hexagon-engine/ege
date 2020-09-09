/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "NetworkEndpoint.h"

#include <ege/loop/EventResult.h>

namespace EGE
{

class Client : public NetworkEndpoint
{
public:
    virtual ~Client() {}

    bool connect(sf::IpAddress ip, unsigned short port);
    void update();
    virtual EventResult onReceive(std::shared_ptr<Packet>) { return EventResult::Failure; }
};

}
