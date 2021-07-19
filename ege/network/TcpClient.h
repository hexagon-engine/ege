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

#include <ege/core/MainLoop.h>

namespace EGE
{

// P must be derived from TcpPacket
template<class P>
class TcpClient : public MainLoop, public TcpEndpoint<P>
{
public:
    using Packet = P;
    virtual ~TcpClient() {}

    bool connect(sf::IpAddress ip, unsigned short port)
    {
        if(TcpEndpoint<P>::socket().connect(ip, port) != sf::Socket::Done)
        {
            ege_log.info() << "TcpClient: Failed to connect to " << ip.toString() << ":" << port;
            return false;
        }
        ege_log.info() << "TcpClient: Connected to " << ip.toString() << ":" << port;
        addAsyncTask(make<AsyncTask>([this](AsyncTask& task) {
            while(true)
            {
                if(!TcpEndpoint<P>::isConnected())
                    return 0;

                auto packet = TcpEndpoint<P>::receive();

                if(packet.isValid())
                    onReceive(packet);
                if(task.stopRequested())
                    return 0;
            }
        }, [this](AsyncTask::State) { ege_log.info() << "TcpClient: Disconnected!"; exit(0); }), "TcpClient");
        return true;
    }

    virtual void onReceive(Packet const&) {}

private:
    SharedPtr<AsyncTask> m_networkTask;
};

}
