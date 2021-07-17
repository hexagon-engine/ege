/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*/

#pragma once

#include <ctime>

namespace EGE
{

namespace System
{

class ExactTime
{
public:
    static inline ExactTime zero() { return {}; }
    static inline ExactTime fromSeconds(time_t s) { return ExactTime(s, 0); }
    static inline ExactTime fromMilliseconds(long long ms) { return ExactTime(ms / 1000, ms * 1000000); }
    static inline ExactTime fromMicroseconds(long long us) { return ExactTime(us / 1000000, us * 1000); }
    static inline ExactTime fromNanoseconds(long long ns) { return ExactTime(ns / 1000000000, ns); }
    static inline ExactTime fromSecondsAndNanoseconds(time_t s, long long ns) { return ExactTime(s, ns % 1000000000); }

    ExactTime roundToSeconds() const { return ExactTime(m_seconds, 0); }
    ExactTime stripSeconds() const { return ExactTime(0, m_nanoseconds); }

    double seconds() const { return m_seconds + m_nanoseconds / 1000000.0; }
    double milliseconds() const { return m_seconds * 1000 + m_nanoseconds / 1000000.0; }
    double microseconds() const { return m_seconds * 1000000 + m_nanoseconds / 1000.0; }
    long long nanoseconds() const { return m_seconds * 1000000000 + m_nanoseconds; }

    time_t onlySeconds() const { return m_seconds; }
    long long onlyNanoseconds() const { return m_nanoseconds; }

private:
    time_t m_seconds;
    long long m_nanoseconds;

    ExactTime(time_t s = 0, long long ns = 0)
    : m_seconds(s), m_nanoseconds(ns) {}
};

// seconds since 1970-01-01
time_t unixTime();

ExactTime exactTime();

}

}
