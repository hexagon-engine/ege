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
#include <ege/network/TcpClientConnection.h>
#include <ege/util/Time.h>
#include <memory>

namespace EGE
{

class EGEClientConnection : public TcpClientConnection<EGEServer>
{
public:
    EGEClientConnection(Server& server);

    void setLastRecvTime(Time t) { m_lastRecv = t; }

    Time getLastRecvTime() const { return m_lastRecv; }
    Time getCreateTime() const { return m_createTime; }

    bool wasPinged() const { return m_pinged; }
    void setPinged(bool ping = true) { m_pinged = ping; }

    SceneObject const* getControlledSceneObject() const { return m_defaultControlledObject; }
    void setControlledSceneObject(const SceneObject* object);

    void addAdditionalController(SceneObject const& object);
    void removeAdditionalController(SceneObject const& object);
    bool hasAdditionalController(SceneObject const& object) const { return m_additionalControlledObjects.count(&object); }

    bool agentVerCheckSucceeded() const { return m_agentVerCheck; }
    bool protVerCheckSucceeded() const { return m_agentVerCheck; }
    void setAgentVerCheckSuccess() { m_agentVerCheck = true; }
    void setProtVerCheckSuccess() { m_agentVerCheck = true; }

    bool canControl(SceneObject const&) const;

    void disconnectWithReason(String reason);

private:
    SceneObject const* m_defaultControlledObject = 0;
    Time m_lastRecv;
    Time m_createTime;
    Set<SceneObject const*> m_additionalControlledObjects;

    bool m_pinged = false;
    bool m_agentVerCheck = false;
    bool m_protVerCheck = false;
};

}
