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

long long ObjectInt::asInt() const
{
    switch(m_type)
    {
    case Type::Byte:
        return m_number & 0xFF;
    case Type::Short:
        return m_number & 0xFFFF;
    case Type::Int:
        return m_number & 0xFFFFFFFF;
    case Type::Long:
        return m_number;
    default:
        CRASH();
    }
}

std::string ObjectInt::toString() const
{
    return asString();
}

std::string ObjectInt::suffix() const
{
    switch(m_type)
    {
    case Type::Byte:
        return "b";
    case Type::Short:
        return "s";
    case Type::Int:
        return "l";
    case Type::Long:
        return "ll";
    }
    CRASH();
}

}
