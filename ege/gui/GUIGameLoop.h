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

#include <ege/core/MainLoop.h>
#include <ege/event/SystemWindow.h>
#include <ege/gfx/RenderStates.h>
#include <ege/gui/Window.h>
#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <ege/resources/ResourceManager.h>
#include <memory>

#define GUI_DEBUG 0

namespace EGE
{

class GUIGameLoop : public MainLoop
{
public:
    GUIGameLoop(String id = "GUIGameLoop")
    : MainLoop(id) {}

    virtual void onTick() override;

    virtual EventResult onFinish(int) override
    {
        return EventResult::Success;
    }

    virtual void logicTick(long long tickCount) {(void) tickCount;}

    // NOTE: it's double-buffered and OpenGL-backed by default!
    virtual void render();

    SharedPtr<ResourceManager> getResourceManager();
    void setResourceManager(SharedPtr<ResourceManager> manager);

    sf::Time getLatestFrameTime() const  { return m_frameTime; }
    double getLastTPS() const { return 1.0 / m_frameTime.asSeconds(); }

    SharedPtr<Window> openWindow(const sf::VideoMode& mode, sf::String label, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());
    SharedPtr<Window> openWindow(sf::WindowHandle handle, const sf::ContextSettings& settings = sf::ContextSettings());

    void setExitOnCloseAllWindows(bool exit) { m_exitOnCloseAllWindows = exit; }

protected:
    virtual EventResult load() { return EventResult::Success; }

private:
    virtual EventResult onLoad() override;

    SharedPtr<ResourceManager> m_resourceManager;
    sf::Clock m_fpsClock;
    sf::Time m_frameTime;
    bool m_exitOnCloseAllWindows = true;
};

}
