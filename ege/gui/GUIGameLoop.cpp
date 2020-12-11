/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "GUIGameLoop.h"

#include "GUIResourceManager.h"

#include <ege/debug/Logger.h>
#include <ege/debug/ProfilerSectionStarter.h>
#include <ege/util/PointerUtils.h>

namespace EGE
{

GUIGameLoop::GUIGameLoop()
: m_renderer(m_systemWindow)
{
}

EventResult GUIGameLoop::onLoad()
{
    // Call user-defined handler
    load();

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
        err(LogLevel::Critical) << "The ResourceManager is not set!";
        m_resourceManager = make<ResourceManager>();
    }

    return EventResult::Success;
    m_fpsClock.restart();
}

void GUIGameLoop::onTick(long long tickCount)
{
    m_profiler->startSection("guiTick");

    // Initialize pending GUI
    m_profiler->startSection("initPendingGUI");
    if(m_pendingGui)
    {
        DBG(GUI_DEBUG, "initPendingGUI");
        if(m_currentGui)
        {
            removeEventHandler(m_currentGui.get());
            // it's 'delete'd by eventhandler
            m_currentGui->onUnload();
        }
        m_currentGui = m_pendingGui;
        m_currentGui->onLoad();

        // allow GUI screens know about window's size when creating
        sf::Vector2u wndSize = getWindow().getSize();
        sf::Event::SizeEvent event{wndSize.x, wndSize.y};
        m_currentGui->onResize(event);

        addEventHandler(SystemEvent::getTypeStatic(), m_currentGui);
        m_pendingGui = nullptr;
    }

    // Call system event handlers
    if(m_systemWindow.isOpen())
    {
        m_profiler->endStartSection("systemEvents");
        DBG(0, "systemEvents");
        m_systemWindow.callEvents(this, SystemWindow::WaitForEvents::No);
    }

    m_profiler->endStartSection("guiUpdate");
    if(m_currentGui)
        m_currentGui->onUpdate(getTickCount());

    m_profiler->endStartSection("logic");
    logicTick(tickCount);

    m_profiler->endStartSection("render");
    render();
    m_profiler->endSection();

    if(!m_systemWindow.isOpen())
        exit();

    // TODO: tick rate limit?
    //log() << m_frameTime.asMicroseconds();
    m_frameTime = m_fpsClock.restart();

    m_profiler->endSection();
}

void GUIGameLoop::setCurrentGUIScreen(std::shared_ptr<GUIScreen> screen, GUIScreenImmediateInit init)
{
    DBG(GUI_DEBUG, "setCurrentGUIScreen");

    if(init == GUIGameLoop::GUIScreenImmediateInit::Yes)
    {
        removeEventHandler(m_currentGui.get());
        // it's 'delete'd by eventhandler
        if(m_currentGui)
           m_currentGui->onUnload();

        m_currentGui = screen;
        m_currentGui->onLoad();

        // allow GUI screens know about window's size when creating
        sf::Vector2u wndSize = getWindow().getSize();
        sf::Event::SizeEvent event{wndSize.x, wndSize.y};
        m_currentGui->onResize(event);

        addEventHandler(SystemEvent::getTypeStatic(), m_currentGui);
    }
    else
        m_pendingGui = screen;
}

SFMLSystemWindow& GUIGameLoop::getWindow()
{
    return m_systemWindow;
}

void GUIGameLoop::openWindow(const sf::VideoMode& mode, sf::String label, sf::Uint32 style, const sf::ContextSettings& settings)
{
    m_systemWindow.create(mode, label, style, settings);

    if(m_currentGui)
    {
        // Notify GUI about size change
        sf::Event::SizeEvent event;
        auto size = m_systemWindow.getSize();
        event.width = size.x;
        event.height = size.y;
        m_currentGui->onResize(event);
    }
}

void GUIGameLoop::openWindow(sf::WindowHandle handle, const sf::ContextSettings& settings)
{
    m_systemWindow.create(handle, settings);

    if(m_currentGui)
    {
        // Notify GUI about size change
        sf::Event::SizeEvent event;
        auto size = m_systemWindow.getSize();
        event.width = size.x;
        event.height = size.y;
        m_currentGui->onResize(event);
    }
}

std::shared_ptr<ResourceManager> GUIGameLoop::getResourceManager()
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
    if(m_systemWindow.isOpen())
    {
        m_profiler->startSection("clear");
        m_systemWindow.clear(m_backgroundColor);

        m_profiler->endStartSection("gui");
        if(m_currentGui)
            m_currentGui->render(m_renderer);

        m_profiler->endStartSection("display");
        m_systemWindow.display();
        m_profiler->endSection();
    }
}

}
