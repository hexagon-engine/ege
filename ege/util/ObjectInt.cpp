/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ObjectInt.h"

namespace EGE
{

std::shared_ptr<Object> ObjectInt::copy() const
{
    return std::make_shared<ObjectInt>(*this);
}

std::string ObjectInt::toString() const
{
    return asString();
}

}
