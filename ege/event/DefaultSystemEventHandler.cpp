/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "DefaultSystemEventHandler.h"

#include <ege/debug/Logger.h>

namespace EGE
{
    void DefaultSystemEventHandler::onClose()
    {
        if(m_window.expired())
        {
            err(LogLevel::Verbose) << "000E EGE/syswindow: window already removed, close skipped";
            return;
        }
        m_window.lock()->close();
    }
}
