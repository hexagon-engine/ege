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

    double getValue() const
    {
        return m_value;
    }
    Unit getUnit() const
    {
        return m_unit;
    }
    Time operator+(double _2);
    Time& operator+=(double _2);

private:
    double m_value;
    Unit m_unit;
};

}
