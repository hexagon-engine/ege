/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

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

    Time(double value, Unit unit)
    : m_value(value)
    , m_unit(unit) {}

    double getValue()
    {
        return m_value;
    }
    Unit getUnit()
    {
        return m_unit;
    }
private:
    double m_value;
    Unit m_unit;
};

}
