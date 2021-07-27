/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/main/Config.h>
#include <ege/util/Types.h>

namespace EGE
{

// TODO: Support actually not giving value to it
template<class T>
class Optional
{
public:
    Optional()
    : m_value(nullptr) {}

    Optional(const T& value)
    : m_value(std::make_unique<T>(value)) {}

    T value() const { ASSERT(m_value); return *m_value; }
    T valueOr(T&& s) const { return m_value ? *m_value : s; }
    bool hasValue() const { return !!m_value; }

private:
    UniquePtr<T> m_value;
};

}
