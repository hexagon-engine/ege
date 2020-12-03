/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/loop/EventLoop.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class SystemWindow
{
public:
    EGE_ENUM_YES_NO(WaitForEvents);
    virtual void callEvents(EventLoop* loop, WaitForEvents wait) = 0;
};

class SFMLSystemWindow : public SystemWindow, public sf::RenderWindow
{
public:
    SFMLSystemWindow()
    : sf::RenderWindow()
    {}

    virtual void callEvents(EventLoop* loop, WaitForEvents wait = WaitForEvents::No);

    bool areGLExtensionsEnabled() { return m_glExtensions; }

private:
    void initialize();

    bool m_glExtensions = false;
};

}
