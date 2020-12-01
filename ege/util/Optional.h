/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/main/Config.h>

namespace EGE
{

template<class T>
class Optional
{
public:
    Optional()
    : m_hasValue(false) {}

    Optional(T value)
    : m_value(value) {}

    T value() { ASSERT(m_hasValue); return m_value; }
    T valueOr(T s) { return m_hasValue ? m_value : s; }
    bool hasValue() { return m_hasValue; }

private:
    T m_value;
    bool m_hasValue = true;
};

}
