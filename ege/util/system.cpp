
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

std::string getEnv(std::string name)
{
    return impl().getEnv(name);
}

std::string name()
{
    return impl().className();
}

} // System

}
