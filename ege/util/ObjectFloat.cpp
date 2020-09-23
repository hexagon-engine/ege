/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ObjectFloat.h"

#include <ege/main/Config.h>

namespace EGE
{

std::shared_ptr<Object> ObjectFloat::copy() const
{
    return make<ObjectFloat>(*this);
}

std::string ObjectFloat::toString() const
{
    return asString() + "f";
}

}
