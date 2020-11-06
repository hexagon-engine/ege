/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ObjectBoolean.h"
#include "PointerUtils.h"

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>

namespace EGE
{

std::shared_ptr<Object> ObjectBoolean::copy() const
{
    return make<ObjectBoolean>(*this);
}

std::string ObjectBoolean::toString() const
{
    return m_value ? "true" : "false";
}

}
