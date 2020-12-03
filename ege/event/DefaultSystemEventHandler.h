/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/event/SystemEvent.h>
#include <ege/event/SystemEventHandler.h>
#include <ege/event/SystemWindow.h>
#include <memory>

namespace EGE
{

class DefaultSystemEventHandler : public SystemEventHandler
{
public:
    DefaultSystemEventHandler(SFMLSystemWindow& window)
    : m_window(window) {}

    virtual void onClose() override;

    SFMLSystemWindow& getWindow() const
    {
        return m_window;
    }

private:
    SFMLSystemWindow& m_window;
};

}
