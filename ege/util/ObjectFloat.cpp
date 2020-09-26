/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ObjectFloat.h"
#include "PointerUtils.h"

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>

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
