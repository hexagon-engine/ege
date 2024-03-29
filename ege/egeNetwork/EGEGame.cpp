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

#include "EGEGame.h"

#include <ege/debug/Logger.h>
#include <functional>

namespace EGE
{


void EGEGame::setScene(SharedPtr<Scene> scene)
{
    m_scene = scene;
}

void EGEGame::registerController(String name, Controller controller)
{
    auto existingController = m_controllers.find(name);
    if(existingController != m_controllers.end())
        ege_log.warning() << "EGEGame: Existing controlller will be overridden: " << name;

    m_controllers.insert(std::make_pair(name, std::move(controller)));
}

void EGEGame::runController(SceneObject& object, const ControlPacket& packet)
{
    auto controller = m_controllers.find(packet.getType());
    if(controller == m_controllers.end()) {
        ege_log.error() << "EGEGame: Unimplemented controlller: " << packet.getType();
        return;
    }
    controller->second(object, packet.getArgs());
}

}
