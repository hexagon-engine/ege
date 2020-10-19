/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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

#define GUI_DEBUG 1

namespace EGE
{

class GUIGameLoop : public GameLoop
{
public:
    //EGE_SINGLETON(GUIGameLoop);
    GUIGameLoop();
    virtual ~GUIGameLoop();
    EGE_ENUM_YES_NO(GUIScreenImmediateInit);

    virtual EventResult onLoad() override;
    virtual void onTick(long long tickCount) override;
    virtual void onExit(int) override {}

    virtual EventResult onFinish(int) override
    {
        return EventResult::Success;
    }

    virtual void logicTick(long long tickCount) {(void) tickCount;}

    // NOTE: it's double-buffered and OpenGL-backed by default!
    virtual void render();

    void setCurrentGUIScreen(std::shared_ptr<GUIScreen> screen, GUIScreenImmediateInit init = EGE::GUIGameLoop::GUIScreenImmediateInit::No);

    std::shared_ptr<GUIScreen> getCurrentGUIScreen()
    {
        return m_currentGui;
    }

    std::weak_ptr<SFMLSystemWindow> getWindow();
    void setWindow(std::shared_ptr<SFMLSystemWindow> window);
    std::weak_ptr<ResourceManager> getResourceManager();
    void setResourceManager(std::shared_ptr<ResourceManager> manager);
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

private:
    std::shared_ptr<GUIScreen> m_currentGui = nullptr;
    // to allow animations and lazy-load
    std::shared_ptr<GUIScreen> m_pendingGui = nullptr;
    std::shared_ptr<SFMLSystemWindow> m_systemWindow;
    std::shared_ptr<ResourceManager> m_resourceManager;
    sf::Color m_backgroundColor;
    sf::Clock m_fpsClock;
    sf::Time m_frameTime;
};

}
