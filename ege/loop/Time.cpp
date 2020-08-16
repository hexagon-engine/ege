

#include "Time.h"

namespace EGE
{

Time Time::operator+(double _2)
{
    return Time(m_value + _2, m_unit);
}

Time& Time::operator+=(double _2)
{
    m_value += _2;
    return *this;
}

}
