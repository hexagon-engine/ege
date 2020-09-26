/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ObjectInt.h"
#include "PointerUtils.h"

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>

namespace EGE
{

std::shared_ptr<Object> ObjectInt::copy() const
{
    return make<ObjectInt>(*this);
}

std::string ObjectInt::toString() const
{
    return asString();
}

}
