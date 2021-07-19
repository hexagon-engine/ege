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

#include "TcpEndpoint.h"

namespace EGE
{

template<class P, class C>
class TcpServer;

// S must be derived from TcpServer
template<class S>
class TcpClientConnection : public TcpEndpoint<typename S::Packet>
{
public:
    using Packet = typename S::Packet;
    using ClientConnection = typename S::ClientConnection;
    using Server = S;

    TcpClientConnection(Server& server)
    : m_server(server) {}

    virtual void disconnect()
    {
        m_server.disconnect(TcpEndpoint<Packet>::socket());
        TcpEndpoint<Packet>::disconnect();
    }

private:
    Server& m_server;
};

}
