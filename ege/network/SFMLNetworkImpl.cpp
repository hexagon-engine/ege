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

#include "SFMLNetworkImpl.h"

#include <ege/debug/Logger.h>
#include <iostream>

namespace EGE
{

bool SFMLNetworkImpl::sendTo(NetworkEndpoint* endpoint, std::shared_ptr<Packet> packet)
{
    // Abort sending empty packet (or TO empty endpoint) with success state.
    if(!packet || !endpoint)
        return true;

    //sf::Lock lock(endpoint->getMutex());
    SFMLPacket* packet2 = (SFMLPacket*)packet.get();

    sf::Packet sfPacket = packet2->toSFMLPacket();
    sf::Socket::Status status = endpoint->getSocket().lock()->send(sfPacket);
    if(status != sf::Socket::Done)
    {
        err(LogLevel::Error) << "0017 EGE/network: Socket Send failed (system error) to (" << endpoint->getSocket().lock()->getRemoteAddress() << ":" << endpoint->getSocket().lock()->getRemotePort() << ")";
        //endpoint->disconnect();
        return false;
    }
    return true;
}

std::shared_ptr<Packet> SFMLNetworkImpl::receiveFrom(NetworkEndpoint* endpoint)
{
    // Abort receiving FROM empty endpoint with empty state.
    if(!endpoint)
        return nullptr;

    //sf::Lock lock(endpoint->getMutex());

    sf::Packet sfPacket;
    sf::Socket::Status status = endpoint->getSocket().lock()->receive(sfPacket);
    if(status != sf::Socket::Done)
    {
        err(LogLevel::Error) << "0016 EGE/network: Socket Receive failed (system error) from (" << endpoint->getSocket().lock()->getRemoteAddress() << ":" << endpoint->getSocket().lock()->getRemotePort() << ")";
        //endpoint->disconnect();
        return nullptr;
    }
    return makePacket(sfPacket);
}

}
