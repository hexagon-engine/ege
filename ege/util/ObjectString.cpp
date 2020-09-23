/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ObjectString.h"

#include <ege/main/Config.h>

namespace EGE
{

std::shared_ptr<Object> ObjectString::copy() const
{
    return make<ObjectString>(*this);
}

std::string ObjectString::toString() const
{
    return "\"" + m_string + "\"";
}

}
