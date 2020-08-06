/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/sysevent/SystemEvent.h>
#include <ege/sysevent/SystemEventHandler.h>
#include <ege/syswindow/SystemWindow.h>
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

private:
    std::weak_ptr<SFMLSystemWindow> m_window;
};

}
