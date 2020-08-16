/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "GUIGameLoop.h"

#include <ege/profiler/ProfilerSectionStarter.h>

namespace EGE
{

GUIGameLoop::GUIGameLoop()
{
    m_profiler = std::make_shared<Profiler>();
}

GUIGameLoop::~GUIGameLoop()
{
}

EventResult GUIGameLoop::onLoad()
{
    ASSERT(m_profiler);
    {
        m_profiler->start();
        ProfilerSectionStarter starter(*m_profiler, "load");
        if(m_resourceManager)
        {
            ProfilerSectionStarter starter2(*m_profiler, "resourceManager");
            bool success = m_resourceManager->reload();
            success &= !m_resourceManager->isError();
            if(!success)
            {
                return EventResult::Failure;
            }
        }
        else
        {
            std::cerr << "000A EGE/gui: no ResourceManager set, setting to default GUIResourceManager" << std::endl;
            // TODO: implement GUIResourceManager
        }
        return EventResult::Success;
    }
    m_fpsClock.restart();
}

void GUIGameLoop::onTick(long long tickCount)
{
    m_profiler->startSection("tick");

    // Initialize pending GUI
    m_profiler->startSection("initPendingGUI");
    if(m_pendingGui)
    {
        DBG(GUI_DEBUG, "initPendingGUI");
        if(m_currentGui)
        {
            removeEventHandler(m_currentGui.get());
            // it's 'delete'd by eventhandler
        }
        m_currentGui = m_pendingGui;
        m_currentGui->onLoad();
        addEventHandler(SystemEvent::getTypeStatic(), m_currentGui);
        m_pendingGui = nullptr;
    }

    // Call system event handlers
    m_profiler->endStartSection("systemEvents");
    if(m_systemWindow)
    {
        DBG(0, "systemEvents");
        m_systemWindow->callEvents(this, SystemWindow::WaitForEvents::No);
    }

    m_profiler->endStartSection("timers");
    updateTimers();

    m_profiler->endStartSection("guiUpdate");
    if(m_currentGui)
        m_currentGui->onUpdate(getTickCount());

    m_profiler->endStartSection("logic");
    logicTick(tickCount);

    m_profiler->endStartSection("render");
    render();
    m_profiler->endSection();

    if(m_systemWindow && !m_systemWindow->isOpen())
        exit();

    // TODO: tick rate limit?
    m_profiler->endSection();
    m_frameTime = m_fpsClock.restart();
}

void GUIGameLoop::setCurrentGUIScreen(std::shared_ptr<GUIScreen> screen, GUIScreenImmediateInit init)
{
    DBG(GUI_DEBUG, "setCurrentGUIScreen");

    if(init == GUIGameLoop::GUIScreenImmediateInit::Yes)
    {
        removeEventHandler(m_currentGui.get());
        // it's 'delete'd by eventhandler
        m_currentGui = screen;
        m_currentGui->onLoad();
        addEventHandler(SystemEvent::getTypeStatic(), m_currentGui);
    }
    else
        m_pendingGui = screen;
}

std::weak_ptr<SFMLSystemWindow> GUIGameLoop::getWindow()
{
    return m_systemWindow;
}

void GUIGameLoop::setWindow(std::shared_ptr<SFMLSystemWindow> window)
{
    DBG(GUI_DEBUG, "setWindow");
    // TODO: automatically deletes previous window!
    // so it forces one GameLoop for window
    // so only one window is allowed currently
    m_systemWindow = std::shared_ptr<SFMLSystemWindow>(window);
}

std::weak_ptr<ResourceManager> GUIGameLoop::getResourceManager()
{
    return m_resourceManager;
}

void GUIGameLoop::setResourceManager(std::shared_ptr<ResourceManager> manager)
{
    m_resourceManager = manager;
}

void GUIGameLoop::setProfiler(std::weak_ptr<Profiler> profiler)
{
    m_profiler = profiler.lock();
}

void GUIGameLoop::render()
{
    if(m_systemWindow)
    {
        m_profiler->startSection("clear");
        m_systemWindow->clear(m_backgroundColor);

        m_profiler->endStartSection("gui");
        if(m_currentGui)
            m_currentGui->render(*(m_systemWindow.get()));

        m_profiler->endStartSection("display");
        m_systemWindow->display();
        m_profiler->endSection();
    }
}

}
