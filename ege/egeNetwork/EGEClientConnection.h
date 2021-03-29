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

#include "EGEPacket.h"
#include "EGEServer.h"

#include <ctime>
#include <ege/network/ClientConnection.h>
#include <ege/network/SFMLNetworkImpl.h>
#include <ege/util/Time.h>
#include <memory>

namespace EGE
{

class EGEClientConnection : public ClientConnection, public SFMLNetworkImpl
{
public:
    EGEClientConnection(EGEServer& server, SharedPtr<sf::TcpSocket> socket)
    : ClientConnection(server, socket)
    , m_lastRecv(server.time(Time::Unit::Seconds))
    , m_createTime(server.time(Time::Unit::Seconds)) {}

    virtual bool send(SharedPtr<Packet> packet)
    {
        return sendTo(this, packet);
    }

    virtual SharedPtr<Packet> receive()
    {
        return receiveFrom(this);
    }

    virtual SharedPtr<SFMLPacket> makePacket(sf::Packet& packet);

    void setLastRecvTime(Time t);

    Time getLastRecvTime()
    {
        return m_lastRecv;
    }

    Time getCreateTime()
    {
        return m_createTime;
    }

    bool wasPinged()
    {
        return m_pinged;
    }
    void setPinged(bool ping = true)
    {
        m_pinged = ping;
    }

    UidType getControlledSceneObject()
    {
        return m_controlledSceneObjectId;
    }
    void setControlledSceneObject(UidType id)
    {
        m_controlledSceneObjectId = id;
    }

    void addAdditionalController(UidType id) { m_additionalControllers.insert(id); }
    void removeAdditionalController(UidType id) { m_additionalControllers.erase(id); }
    bool hasAdditionalController(UidType id) { return m_additionalControllers.count(id); }

    bool agentVerCheckSucceeded() { return m_agentVerCheck; }
    bool protVerCheckSucceeded() { return m_agentVerCheck; }
    void setAgentVerCheckSuccess() { m_agentVerCheck = true; }
    void setProtVerCheckSuccess() { m_agentVerCheck = true; }

private:
    UidType m_controlledSceneObjectId = 0;
    Time m_lastRecv;
    Time m_createTime;
    Set<UidType> m_additionalControllers;

    bool m_pinged = false;
    bool m_agentVerCheck = false;
    bool m_protVerCheck = false;
};

}
