/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/loop/GameLoop.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class SystemWindow
{
public:
    EGE_ENUM_YES_NO(WaitForEvents);
    virtual void callEvents(GameLoop* loop, WaitForEvents wait) = 0;
};

class SFMLSystemWindow : public SystemWindow, public sf::RenderWindow
{
public:
    SFMLSystemWindow()
    : sf::RenderWindow()
    {}

    SFMLSystemWindow(sf::VideoMode mode, const sf::String& title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings())
    : sf::RenderWindow(mode, title, style, settings)
    {}

    explicit SFMLSystemWindow(sf::WindowHandle handle, const sf::ContextSettings& settings = sf::ContextSettings())
    : sf::RenderWindow(handle, settings)
    {}

    virtual void callEvents(GameLoop* loop, WaitForEvents wait = WaitForEvents::No);
};

}
