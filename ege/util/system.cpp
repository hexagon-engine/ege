
#include "system.h"

#include "system/SystemImpl.h"

namespace EGE
{

namespace System
{

std::string getErrorMessage()
{
    return impl().getErrorMessage();
}

} // System

}
