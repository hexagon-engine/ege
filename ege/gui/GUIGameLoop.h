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

#include "GUIScreen.h"

#include <ege/event/SystemWindow.h>
#include <ege/gfx/RenderStates.h>
#include <ege/gui/GameLoop.h>
#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <ege/resources/ResourceManager.h>
#include <memory>

#define GUI_DEBUG 0

namespace EGE
{

class GUIGameLoop : public GameLoop
{
public:
    //EGE_SINGLETON(GUIGameLoop);

    GUIGameLoop(String id = "GUIGameLoop")
    : GameLoop(id), m_renderer(m_systemWindow) {}

    EGE_ENUM_YES_NO(GUIScreenImmediateInit);

    virtual void onTick(long long tickCount) override;
    virtual void onExit(int) override {}

    virtual EventResult onFinish(int) override
    {
        return EventResult::Success;
    }

    virtual void logicTick(long long tickCount) {(void) tickCount;}

    // NOTE: it's double-buffered and OpenGL-backed by default!
    virtual void render();

    void setCurrentGUIScreen(SharedPtr<GUIScreen> screen, GUIScreenImmediateInit init = EGE::GUIGameLoop::GUIScreenImmediateInit::No);

    SharedPtr<GUIScreen> getCurrentGUIScreen() { return m_currentGui; }

    SFMLSystemWindow& getWindow();
    Renderer& getRenderer() { return m_renderer; }

    void openWindow(const sf::VideoMode& mode, sf::String label, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());
    void openWindow(sf::WindowHandle handle, const sf::ContextSettings& settings = sf::ContextSettings());

    SharedPtr<ResourceManager> getResourceManager();
    void setResourceManager(SharedPtr<ResourceManager> manager);
    void setProfiler(std::weak_ptr<Profiler> profiler);

    void setBackgroundColor(sf::Color color)
    {
        m_backgroundColor = color;
    }
    sf::Color getBackgroundColor()
    {
        return m_backgroundColor;
    }
    sf::Time getLatestFrameTime()
    {
        return m_frameTime;
    }

protected:
    virtual EventResult load() { return EventResult::Success; }

private:
    virtual EventResult onLoad() override;

    SharedPtr<GUIScreen> m_currentGui;
    // to allow animations and lazy-load
    SharedPtr<GUIScreen> m_pendingGui;
    SharedPtr<ResourceManager> m_resourceManager;
    SFMLSystemWindow m_systemWindow;
    Renderer m_renderer;

    sf::Color m_backgroundColor;
    sf::Clock m_fpsClock;
    sf::Time m_frameTime;
};

}
