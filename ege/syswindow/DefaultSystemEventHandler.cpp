/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "DefaultSystemEventHandler.h"

namespace EGE
{
    void DefaultSystemEventHandler::onClose()
    {
        ASSERT(!m_window.expired())
        m_window.lock()->close();
    }
}
