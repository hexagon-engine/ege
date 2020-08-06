/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "GUIGameLoop.h"

namespace EGE
{

GUIGameLoop::GUIGameLoop()
{
}

void GUIGameLoop::onTick(long long tickCount)
{
    m_profiler.startSection("tick");

    // Initialize pending GUI
    m_profiler.startSection("initPendingGUI");
    if(m_pendingGui)
    {
        DBG(GUI_DEBUG, "initPendingGUI");
        m_currentGui = m_pendingGui;
        addEventHandler(SystemEvent::getTypeStatic(), std::shared_ptr<GUIScreen>(m_currentGui));
        m_pendingGui = NULL;
    }

    // Call system event handlers
    m_profiler.endStartSection("systemEvents");
    if(m_systemWindow)
    {
        m_systemWindow->callEvents(this, SystemWindow::WaitForEvents::No);
    }

    m_profiler.endStartSection("logic");
    logicTick(tickCount);

    m_profiler.endStartSection("render");
    render();
    m_profiler.endSection();

    if(!m_systemWindow->isOpen())
        exit();

    // TODO: tick rate limit?

    m_profiler.endSection();
}

void GUIGameLoop::setCurrentGUIScreen(GUIScreen* screen, GUIScreenImmediateInit init)
{
    DBG(GUI_DEBUG, "setCurrentGUIScreen");
    if(m_currentGui)
        removeEventHandler(m_currentGui);

    if(init == GUIGameLoop::GUIScreenImmediateInit::Yes)
    {
        m_currentGui = screen;
        addEventHandler(SystemEvent::getTypeStatic(), std::shared_ptr<GUIScreen>(m_currentGui));
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

void GUIGameLoop::render()
{
    if(m_systemWindow)
    {
        m_profiler.startSection("clear");
        m_systemWindow->clear();

        m_profiler.endStartSection("gui");
        if(m_currentGui)
            m_currentGui->render(*(m_systemWindow.get()));

        m_profiler.endStartSection("display");
        m_systemWindow->display();
        m_profiler.endSection();
    }
}

}
