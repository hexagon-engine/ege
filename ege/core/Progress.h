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

#include <ege/core/AsyncTask.h>
#include <ege/util/Types.h>

namespace EGE
{

class Progress
{
public:
    explicit Progress(size_t maxStepCount)
    : m_maxStepCount(maxStepCount) {}

    void setTask(SharedPtr<AsyncTask> task) { m_task = task;}
    inline void step() { m_stepCount++; }
    inline void setError() { m_error = true; }

    constexpr size_t getStepCount() const { return m_stepCount; }
    constexpr size_t getMaxStepCount() const { return m_maxStepCount; }
    constexpr size_t getRemainingStepCount() const { return getMaxStepCount() - getStepCount(); }
    constexpr float getFactor() const { return static_cast<float>(m_stepCount) / m_maxStepCount; }
    constexpr float getPercent() const { return getFactor() * 100; }

    constexpr bool error() const { return m_error; }
    constexpr bool finished() const { return m_stepCount == m_maxStepCount; }
    SharedPtr<AsyncTask> task() const { return m_task; }

private:
    size_t m_maxStepCount = 0;
    size_t m_stepCount = 0;
    bool m_error = false;
    SharedPtr<AsyncTask> m_task;
};

inline std::ostream& operator<<(std::ostream& stream, const Progress& arg)
{
    return stream << arg.getPercent() << "%";
}

}
