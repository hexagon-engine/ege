#include "DefaultSystemEventHandler.h"

namespace EGE
{
    void DefaultSystemEventHandler::onClose()
    {
        ASSERT(!m_window.expired())
        m_window.lock()->close();
    }
}
