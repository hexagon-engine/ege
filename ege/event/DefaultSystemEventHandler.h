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
    DefaultSystemEventHandler(std::weak_ptr<SFMLSystemWindow> window)
    : m_window(window)
    {}

    virtual void onClose() override;

    std::weak_ptr<SFMLSystemWindow> getWindow() const
    {
        return m_window;
    }
    void setWindow(std::weak_ptr<SFMLSystemWindow> window)
    {
        m_window = window;
    }

private:
    std::weak_ptr<SFMLSystemWindow> m_window;
};

}
