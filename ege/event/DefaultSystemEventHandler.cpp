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
        m_window.close();
    }
}
