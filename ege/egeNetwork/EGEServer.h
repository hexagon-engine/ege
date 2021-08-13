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

#include "EGEGame.h"
#include "EGEPacket.h"

#include <ege/network/TcpServer.h>
#include <ege/util/ObjectMap.h>
#include <memory>

#define EGESERVER_DEBUG 0
#define PING_DEBUG 0

namespace EGE
{

class EGEClientConnection;

class EGEServer : public TcpServer<EGEPacket, EGEClientConnection>, public EGEGame
{
public:
    virtual void setScene(SharedPtr<Scene> scene) override;

    virtual void onClientConnect(ClientConnection& client) override;
    virtual void onClientDisconnect(ClientConnection& client) override;
    virtual void onReceive(ClientConnection& client, Packet const& packet) override;

    // to be overridden by user
    // args: client, reason message
    virtual void onClientDisconnect(ClientConnection&, std::string) {}

    virtual SharedPtr<ObjectMap> getLoginData(ClientConnection&) { return nullptr; }

    // Accept all logins by default, without doing anything with userData!
    // Arguments:                     client                userData
    virtual bool acceptLoginData(ClientConnection&, SharedPtr<ObjectMap>) { return true; }
    virtual void onLogin(ClientConnection&) {}

    virtual void onTick() override;

    void control(SceneObject const& object, String action, SharedPtr<ObjectMap> data);
};

}
