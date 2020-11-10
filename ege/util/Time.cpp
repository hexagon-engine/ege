/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Time.h"

namespace EGE
{

Time Time::operator+(MaxFloat _2)
{
    return Time(m_value + _2, m_unit);
}

Time& Time::operator+=(MaxFloat _2)
{
    m_value += _2;
    return *this;
}

}
