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

#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <SFML/Network.hpp>

#include <ege/debug/Logger.h>
#include <ege/util/Types.h>

#include "TcpPacket.h"

namespace EGE
{

// P must be derived from TcpPacket
template<class P>
class TcpEndpoint
{
public:
    using Packet = P;

    TcpEndpoint() = default;
    TcpEndpoint(const TcpEndpoint&) = delete;
    TcpEndpoint(TcpEndpoint&&) = delete;

    virtual ~TcpEndpoint() { disconnect(); }

    bool send(Packet const& packet)
    {
        return packet.send(m_socket);
    }
    Packet receive()
    {
        if(!isConnected())
            return {};

        Packet packet;
        if(!packet.receive(m_socket))
        {
            disconnect();
            return {};
        }
        return std::move(packet);
    }

    bool isConnected() const { return m_connected.load(); }

    virtual void disconnect()
    {
        if(!m_connected.load())
            return;

        m_connected = false;
        onDisconnect();
        m_socket.disconnect();
    }

    const sf::TcpSocket& socket() const { return m_socket; }
    sf::TcpSocket& socket() { return m_socket; }

    std::string toString() const
    {
        return "TcpEndpoint{"
        + m_socket.getRemoteAddress().toString()
        + ":"
        + std::to_string(m_socket.getRemotePort())
        + "}";
    }

    std::string getIPAddressString() const { return socket().getRemoteAddress().toString(); }
    Uint16 port() const { return socket().getRemotePort(); }

    virtual void onDisconnect() {}

protected:
    sf::TcpSocket m_socket;
    std::atomic<bool> m_connected = true;
};

}
