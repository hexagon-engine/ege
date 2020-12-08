/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Types.h"

namespace EGE
{

class Time
{
public:
    enum class Unit
    {
        Ticks,
        Seconds
    };

    Time()
    : m_value(0)
    , m_unit(Unit::Seconds) {}

    Time(MaxFloat value, Unit unit)
    : m_value(value)
    , m_unit(unit) {}

    MaxFloat getValue() const
    {
        return m_value;
    }
    Unit getUnit() const
    {
        return m_unit;
    }
    Time operator+(MaxFloat _2);
    Time& operator+=(MaxFloat _2);

private:
    MaxFloat m_value;
    Unit m_unit;
};

}
