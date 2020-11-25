/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ObjectUnsignedInt.h"
#include "PointerUtils.h"

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>

namespace EGE
{

std::shared_ptr<Object> ObjectUnsignedInt::copy() const
{
    return make<ObjectUnsignedInt>(*this);
}

unsigned long long ObjectUnsignedInt::asUnsignedInt() const
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

std::string ObjectUnsignedInt::toString() const
{
    return asString();
}

std::string ObjectUnsignedInt::suffix() const
{
    switch(m_type)
    {
    case Type::Byte:
        return "ub";
    case Type::Short:
        return "us";
    case Type::Int:
        return "ul";
    case Type::Long:
        return "ull";
    }
    CRASH();
}

}
