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

#include "SFMLPacket.h"

#include <ege/debug/Logger.h>
#include <string.h>

namespace EGE
{

bool SFMLPacket::send(sf::TcpSocket& socket) const
{
    sf::Packet packet = serializeToSFML();
    return socket.send(packet) == sf::Socket::Done;
}

bool SFMLPacket::receive(sf::TcpSocket& socket)
{
    sf::Packet packet;
    sf::SocketSelector selector;
    selector.add(socket);
    // TODO: Make timeout configurable
    if(selector.wait(sf::milliseconds(500)) && selector.isReady(socket))
    {
        auto status = socket.receive(packet);
        if(status != sf::Socket::Done)
        {
            ege_log.error() << "An error occured receiving packet (" << strerror(errno) << ")";
            return false;
        }
        if(deserializeFromSFML(packet))
            setValid();
        else
        {
            ege_log.error() << "An error occured deserializing packet";
            return false;
        }
        return true;
    }
    else
    {
        // Just do nothing.
        return true;
    }
}

}
