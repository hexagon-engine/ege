/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "DefaultSystemEventHandler.h"

namespace EGE
{
    void DefaultSystemEventHandler::onClose()
    {
        if(m_window.expired())
        {
            std::cerr << "000E EGE/syswindow: window already removed, close skipped" << std::endl;
            return;
        }
        m_window.lock()->close();
    }
}
