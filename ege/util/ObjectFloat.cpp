/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ObjectFloat.h"

namespace EGE
{

std::shared_ptr<Object> ObjectFloat::copy() const
{
    return std::make_shared<ObjectFloat>(*this);
}

std::string ObjectFloat::toString() const
{
    return asString() + "f";
}

}
