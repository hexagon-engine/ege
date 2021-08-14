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

#include "GUIGameLoop.h"

#include "GUIResourceManager.h"

#include <ege/debug/Logger.h>
#include <ege/debug/ProfilerSectionStarter.h>
#include <ege/util/PointerUtils.h>

namespace EGE
{

EventResult GUIGameLoop::onLoad()
{
    // Call user-defined handler
    if(load() == EventResult::Failure)
        return EventResult::Failure;

    // Do our own initializations
    if(m_resourceManager)
    {
        bool success = m_resourceManager->reload();
        success &= !m_resourceManager->isError();
        if(!success)
        {
            return EventResult::Failure;
        }
    }
    else
    {
        // Print a message and create some default resource manager (to not crash Scene)
        ege_log.warning() << "GUIGameLoop: No ResourceManager set, using default dummy one";
        m_resourceManager = make<ResourceManager>();
    }

    return EventResult::Success;
    m_fpsClock.restart();
}

void GUIGameLoop::onTick()
{
    auto profiler = getProfiler();
    ProfilerSectionStarter starter(*profiler, "GUIGameLoop/exitOnCloseAllWindowsCheck");

    bool isAtLeastOneWindowOpen = false;
    forEachChildOfType<Window>([&](auto& window)->void {
        if(window.isOpen())
            isAtLeastOneWindowOpen = true;
    });

    if(!isAtLeastOneWindowOpen && m_exitOnCloseAllWindows)
        exit();

    starter.switchSection("GUIGameLoop/logic");
    logicTick(getTickCount());

    starter.switchSection("GUIGameLoop/render");
    render();

    // TODO: tick rate limit?
    //log() << m_frameTime.asMicroseconds();
    m_frameTime = m_fpsClock.restart();
}

SharedPtr<Window> GUIGameLoop::openWindow(const sf::VideoMode& mode, sf::String label, sf::Uint32 style, const sf::ContextSettings& settings)
{
    auto window = addNewChild<Window>(*this, "Window: " + label.toAnsiString());
    window->create(mode, label, style, settings);
    return window;
}

SharedPtr<Window> GUIGameLoop::openWindow(sf::WindowHandle handle, const sf::ContextSettings& settings)
{
    auto window = addNewChild<Window>(*this, "Window (external)");
    window->create(handle, settings);
    return window;
}

SharedPtr<ResourceManager> GUIGameLoop::getResourceManager()
{
    return m_resourceManager;
}

void GUIGameLoop::setResourceManager(SharedPtr<ResourceManager> manager)
{
    m_resourceManager = manager;
}

void GUIGameLoop::render()
{
    size_t counter = 0;
    forEachChildOfType<Window>([&](auto& window)->void {
        ProfilerSectionStarter starter(*getProfiler(), "GUIGameLoop/render/" + std::to_string(counter));
        window.render();
        counter++;
    });
}

}
