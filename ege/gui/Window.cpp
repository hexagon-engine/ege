#include "Window.h"

#include <ege/gui/GUIGameLoop.h>

namespace EGE
{

Window::Window(GUIGameLoop& owner, String id)
: EventLoop(&owner, id), m_renderer(*this), m_loop(owner) {}

void Window::setGUIScreen(SharedPtr<GUIScreen> screen, GUIScreenImmediateInit init)
{
    if(init == GUIScreenImmediateInit::Yes)
    {
        // FIXME: Handle code dupe with onTick()
        // FIXME: this is almost never used, why does it exist?
        events<SystemEvent>().remove(*m_currentGui);

        // it's 'delete'd by eventhandler
        if(m_currentGui)
           m_currentGui->onUnload();

        m_currentGui = screen;
        m_currentGui->onCreate();

        // allow GUI screens know about window's size when creating
        sf::Vector2u wndSize = getSize();
        sf::Event::SizeEvent event{wndSize.x, wndSize.y};
        m_currentGui->onResize(event);

        events<SystemEvent>().addHandler(m_currentGui);
    }
    else
        m_pendingGui = screen;
}

void Window::onTick(long long)
{
    m_profiler = getGUILoop().getProfiler();

    // Initialize pending GUI
    m_profiler->startSection("initPendingGUI");
    if(m_pendingGui)
    {
        // FIXME: Handle code dupe with immediate-init path of setGUIScreen()
        if(m_currentGui)
        {
            events<SystemEvent>().remove(*m_currentGui);
            // it's 'delete'd by eventhandler
            m_currentGui->onUnload();
        }
        m_currentGui = m_pendingGui;
        m_currentGui->onCreate();

        // allow GUI screens know about window's size when creating
        sf::Vector2u wndSize = getSize();
        sf::Event::SizeEvent event{wndSize.x, wndSize.y};
        m_currentGui->onResize(event);

        events<SystemEvent>().addHandler(m_currentGui);
        m_pendingGui = nullptr;
    }

    // Call system event handlers
    if(isOpen())
    {
        m_profiler->endStartSection("systemEvents");
        callEvents(*this, SystemWindow::WaitForEvents::No);
    }

    m_profiler->endStartSection("guiUpdate");
    if(m_currentGui)
        m_currentGui->onUpdate(getTickCount());
}

void Window::render()
{
    if(isOpen())
    {
        setActive(true);
        m_profiler->startSection("clear");
        clear({m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b});

        m_profiler->endStartSection("gui");
        if(m_currentGui)
            m_currentGui->doRender(m_renderer);

        m_profiler->endStartSection("display");
        display();
        m_profiler->endSection();
    }
}

void Window::onCreate()
{
    if(m_currentGui)
    {
        // Notify GUI about size change
        sf::Event::SizeEvent event;
        auto size = getSize();
        event.width = size.x;
        event.height = size.y;
        m_currentGui->onResize(event);
    }
}


}
