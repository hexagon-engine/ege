/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <memory>
#include <sysevent/SystemEvent.h>
#include <sysevent/SystemEventHandler.h>
#include <syswindow/SystemWindow.h>

namespace EGE
{

class DefaultSystemEventHandler : public SystemEventHandler
{
public:
    DefaultSystemEventHandler(std::weak_ptr<SFMLSystemWindow> window)
    : m_window(window)
    {}

    virtual void onClose() override;

private:
    std::weak_ptr<SFMLSystemWindow> m_window;
};

}
