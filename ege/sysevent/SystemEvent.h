/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/loop/Event.h>
#include <SFML/Window.hpp>

namespace EGE
{

class SystemEvent : public Event
{
    EGE_EVENT("EGE::SystemEvent");
public:
    explicit SystemEvent(sf::Event event)
        : m_event(event)
    {}

    const sf::Event& getEvent() { return m_event; }

private:
    sf::Event m_event;
};

}
