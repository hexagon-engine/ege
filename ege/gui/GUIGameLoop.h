/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "GUIScreen.h"

#include <ege/loop/GameLoop.h>
#include <ege/main/Config.h>
#include <ege/profiler/Profiler.h>
#include <ege/resources/ResourceManager.h>
#include <ege/syswindow/SystemWindow.h>
#include <memory>

#define GUI_DEBUG 0

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
    virtual void onExit(int exitCode) override {}
    virtual EventResult onFinish(int exitCode) override
    {
        // TODO: save it to file instead of std::cerr
        m_profiler.end();
        std::cerr << m_profiler.toString() << std::endl;
        return EventResult::Success;
    }
    virtual void logicTick(long long tickCount) {(void) tickCount;}

    // NOTE: it's double-buffered and OpenGL-backed by default!
    virtual void render();

    void setCurrentGUIScreen(std::shared_ptr<GUIScreen> screen, GUIScreenImmediateInit init = EGE::GUIGameLoop::GUIScreenImmediateInit::No);
    std::weak_ptr<SFMLSystemWindow> getWindow();
    void setWindow(std::shared_ptr<SFMLSystemWindow> window);
    std::weak_ptr<ResourceManager> getResourceManager();
    void setResourceManager(std::shared_ptr<ResourceManager> manager);

protected:
    Profiler m_profiler;

private:
    std::shared_ptr<GUIScreen> m_currentGui = nullptr;
    // to allow animations and lazy-load
    std::shared_ptr<GUIScreen> m_pendingGui = nullptr;
    std::shared_ptr<SFMLSystemWindow> m_systemWindow;
    std::shared_ptr<ResourceManager> m_resourceManager;
};

}
